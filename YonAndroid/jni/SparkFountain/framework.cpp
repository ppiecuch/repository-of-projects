#include "SPK.h"
#include "SPK_YON.h"
using namespace SPK;
using namespace SPK::YON;
#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
IRandomizer* randomizer=NULL;
ITimer* timer=NULL;

float angleY = 0.0f;
float angleX = 45.0f;
float camPosZ = 5.0f;

bool highGraphics = true;

Group* particleGroup = NULL;
System* particleSystem = NULL;
// setup some useful variables
float time,oldtime,deltaTime;
int totalTime = 0;
float step = 0.0f;

// Emitters
const int NB_EMITTERS = 13;
StraightEmitter* particleEmitter[NB_EMITTERS];

float flow[NB_EMITTERS] =
{
	50.0f,

	60.0f,
	60.0f,
	60.0f,
	60.0f,

	90.0f,
	90.0f,
	90.0f,
	90.0f,
	90.0f,
	90.0f,
	90.0f,
	90.0f,
};

float flowLow[NB_EMITTERS] =
{
	15.0f,

	20.0f,
	20.0f,
	20.0f,
	20.0f,

	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
};

f32 random(f32 a, f32 b){
	return randomizer->rand((int)(a*100),(int)(b*100))/100.f;
}

// Call back function to transform water particles that touches the water into splash particles
bool splash(Particle& particle,float deltaTime)
{
	if (particle.position().y < 0.1f)
	{
		if (particle.velocity().y > -0.5f)
			return true;

		particle.position().y = 0.1f;
		particle.position().x += random(0.0f,0.2f) - 0.1f;
		particle.position().z += random(0.0f,0.2f) - 0.1f;

		particle.velocity().set(0,-random(0.1f,0.4f) * particle.velocity().y,0);

		particle.setParamCurrentValue(PARAM_ALPHA,0.4f);
		particle.setParamCurrentValue(PARAM_SIZE,0.0f);

		particle.setLifeLeft(0.5f);
	}

	return false;
}

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(evt.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_DOZE:
				YON_DEBUG("DOZE\r\n");
				break;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				YON_DEBUG("WAKE\r\n");
				break;
			}
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				YON_DEBUG("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				YON_DEBUG("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				highGraphics=!highGraphics;
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//YON_DEBUG("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//YON_DEBUG("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	Logger->setAppender(MASK_APPENDER_VS|MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE|MASK_APPENDER_SCREEN);
	//Logger->setFormat(MASK_FORMAT_LOG);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCameraFPS(NULL,0.01f);
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/spark");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/spark");
#endif

	// Sets the update step
	System::setClampStep(true,0.1f);			// clamp the step to 100 ms
	System::useAdaptiveStep(0.001f,0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)

	// Inits Particle Engine
	Vector3D gravity(0.0f,-2.2f,0.0f);

	// Renderer
	YONPointRenderer* basicRenderer = YONPointRenderer::create(engine);

	YONQuadRenderer* particleRenderer = YONQuadRenderer::create(engine);
	particleRenderer->setScale(0.06f,0.06f);
	particleRenderer->setTexturingMode(TEXTURE_2D);
	particleRenderer->setTexture(videoDriver->getTexture("waterdrops.png"));
	particleRenderer->setBlending(BLENDING_ALPHA);
	particleRenderer->enableRenderingHint(DEPTH_WRITE,false);

	// Model
	Model* particleModel = Model::create(FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE,
		FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE,
		FLAG_SIZE | FLAG_ANGLE);

	particleModel->setLifeTime(1.6f,2.2f);
	particleModel->setParam(PARAM_ALPHA,0.2f,0.0f);
	particleModel->setParam(PARAM_SIZE,1.0f,1.0f,2.0f,8.0f);
	particleModel->setParam(PARAM_ANGLE,0.0f,4.0f * PI,0.0f,4.0f * PI);

	

	Point* emitterZone[NB_EMITTERS];
	emitterZone[0] = Point::create(Vector3D(0.0f,0.1f,0.0f));

	emitterZone[1] = Point::create(Vector3D(0.0f,0.1f,0.0f));
	emitterZone[2] = Point::create(Vector3D(0.0f,0.1f,0.0f));
	emitterZone[3] = Point::create(Vector3D(0.0f,0.1f,0.0f));
	emitterZone[4] = Point::create(Vector3D(0.0f,0.1f,0.0f));

	emitterZone[5] = Point::create(Vector3D(-1.6f,0.1f,-1.6f));
	emitterZone[6] = Point::create(Vector3D(1.6f,0.1f,1.6f));
	emitterZone[7] = Point::create(Vector3D(1.6f,0.1f,-1.6f));
	emitterZone[8] = Point::create(Vector3D(-1.6f,0.1f,1.6f));
	emitterZone[9] = Point::create(Vector3D(-2.26f,0.1f,0.0f));
	emitterZone[10] = Point::create(Vector3D(2.26f,0.1f,0.0f));
	emitterZone[11] = Point::create(Vector3D(0.0f,0.1f,-2.26f));
	emitterZone[12] = Point::create(Vector3D(0.0f,0.1f,2.26f));

	particleEmitter[0] = StraightEmitter::create(Vector3D(0.0f,1.0f,0.0f));

	particleEmitter[1] = StraightEmitter::create(Vector3D(1.0f,3.0f,1.0f));
	particleEmitter[2] = StraightEmitter::create(Vector3D(-1.0f,3.0f,-1.0f));
	particleEmitter[3] = StraightEmitter::create(Vector3D(-1.0f,3.0f,1.0f));
	particleEmitter[4] = StraightEmitter::create(Vector3D(1.0f,3.0f,-1.0f));

	particleEmitter[5] = StraightEmitter::create(Vector3D(1.0f,2.0f,1.0f));
	particleEmitter[6] = StraightEmitter::create(Vector3D(-1.0f,2.0f,-1.0f));
	particleEmitter[7] = StraightEmitter::create(Vector3D(-1.0f,2.0f,1.0f));
	particleEmitter[8] = StraightEmitter::create(Vector3D(1.0f,2.0f,-1.0f));
	particleEmitter[9] = StraightEmitter::create(Vector3D(1.41f,2.0f,0.0f));
	particleEmitter[10] = StraightEmitter::create(Vector3D(-1.41f,2.0f,0.0f));
	particleEmitter[11] = StraightEmitter::create(Vector3D(0.0f,2.0f,1.41f));
	particleEmitter[12] = StraightEmitter::create(Vector3D(0.0f,2.0f,-1.41f));

	

	for (int i = 0; i < NB_EMITTERS; ++i)
	{
		particleEmitter[i]->setZone(emitterZone[i]);
		particleEmitter[i]->setFlow(flow[i]);
		particleEmitter[i]->setForce(2.5f,4.0f);
	}
	particleEmitter[0]->setForce(3.0f,3.5f);

	// Group
	particleGroup = Group::create(particleModel,20000);
	particleGroup->setRenderer(particleRenderer);
	for (int i = 0; i < NB_EMITTERS; ++i)
		particleGroup->addEmitter(particleEmitter[i]);
	particleGroup->setCustomUpdate(&splash);
	particleGroup->setGravity(gravity);
	particleGroup->setFriction(0.7f);

	// System
	particleSystem = System::create();
	particleSystem->addGroup(particleGroup);

	// setup some useful variables
	time=(f32)timer->getTime() / 1000.0f;

	SPKFactory::getInstance().traceAll();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin(true,true,COLOR_DEFAULT);

	oldtime = time;
	time = (f32)timer->getTime() / 1000.0f;
	deltaTime = time - oldtime;

	// Update emitters
	float angle = totalTime * 2.0f * PI / 15000.0f;
	float sinA = sin(angle);
	float cosA = cos(angle);
	float sinB = sin(angle * 10.0f);

	for (int i = 1; i < 5; ++i)
	{
		particleEmitter[i]->setForce(1.8f + 1.8f * sinB,2.1f + 2.1f * sinB);
		if (highGraphics)
			particleEmitter[i]->setFlow((flow[i] * 0.5f) + (flow[i] * 0.5f) * sinB);
		else
			particleEmitter[i]->setFlow((flowLow[i] * 0.5f) + (flowLow[i] * 0.5f) * sinB);
	}

	particleEmitter[1]->setDirection(Vector3D(cosA - sinA,3.0f,sinA + cosA));
	particleEmitter[2]->setDirection(Vector3D(-cosA + sinA,3.0f,-sinA - cosA));
	particleEmitter[3]->setDirection(Vector3D(-cosA - sinA,3.0f,-sinA + cosA));
	particleEmitter[4]->setDirection(Vector3D(cosA + sinA,3.0f,sinA - cosA));

	// Updates particle system
	particleSystem->update(deltaTime); // 1 defined as a second

	totalTime += deltaTime;

	// Renders particle system
	particleSystem->render();

	sceneMgr->render(videoDriver);
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,PTL:%d,DWL:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),particleSystem->getNbParticles(),videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	videoDriver->end();
}
void destroy(){
	SPKFactory::getInstance().destroyAll();
	SPKFactory::destroyInstance();
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}