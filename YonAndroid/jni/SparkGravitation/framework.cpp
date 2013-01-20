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

// setup some useful variables
float curtime,oldtime,deltaTime;
float step = 0.0f;

Group* particleGroup = NULL;
Group* massGroup = NULL;
System* particleSystem = NULL;

Model* massModel = NULL;
Model* particleModel= NULL;

const size_t NB_SEGMENTS = 32;
// the number of point masses
const unsigned int NB_POINT_MASS = 2;
// Creates the point masses that will atract the particles
PointMass* pointMasses[NB_POINT_MASS];

// Converts a HSV color to RGB
// h E [0,360]
// s E [0,1]
// v E [0,1]
Vector3D convertHSV2RGB(const Vector3D& hsv)
{
	float h = hsv.x;
	float s = hsv.y;
	float v = hsv.z;

	int hi = static_cast<int>(h / 60.0f) % 6;
	float f = h / 60.0f - hi;
	float p = v * (1.0f - s);
	float q = v * (1.0f - f * s);
	float t = v * (1.0f - (1.0f - f) * s);

	switch(hi)
	{
	case 0 : return Vector3D(v,t,p);
	case 1 : return Vector3D(q,v,p);
	case 2 : return Vector3D(p,v,t);
	case 3 : return Vector3D(p,q,v);
	case 4 : return Vector3D(t,p,v);
	default : return Vector3D(v,p,q);
	}
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
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
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
	// Renderers
	YONPointRenderer* basicRenderer = YONPointRenderer::create(engine);

	//TODO
	//YONLineTrailRenderer* trailRenderer = GLLineTrailRenderer::create();
	YONLineRenderer* trailRenderer = YONLineRenderer::create(engine);
	trailRenderer->setBlending(BLENDING_ADD);
	//trailRenderer->setDuration(2.0);
	//trailRenderer->setNbSamples(NB_SEGMENTS);
	trailRenderer->enableRenderingHint(DEPTH_WRITE,false);
	trailRenderer->enableRenderingHint(DEPTH_TEST,false);

	// Models
	massModel = Model::create();
	massModel->setImmortal(true);

	particleModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA,
		FLAG_ALPHA,
		FLAG_RED | FLAG_GREEN | FLAG_BLUE);
	particleModel->setParam(PARAM_ALPHA,0.1f,0.0f);
	particleModel->setLifeTime(5.0f,8.0f);

	// Emitter
	RandomEmitter* particleEmitter = RandomEmitter::create();
	particleEmitter->setFlow(400);
	particleEmitter->setForce(0.0f,0.1f);


	// This is the point mass that will attract the other point masses to make them move
	PointMass* centerPointMass = PointMass::create();
	centerPointMass->setMass(0.6f);
	centerPointMass->setMinDistance(0.01f);

	// Groups
	particleGroup = Group::create(particleModel,4100);
	particleGroup->addEmitter(particleEmitter);
	particleGroup->setRenderer(trailRenderer);

	massGroup = Group::create(massModel,NB_POINT_MASS);
	massGroup->addModifier(centerPointMass);
	massGroup->setRenderer(NULL);

	
	for (int i = 0; i < NB_POINT_MASS; ++i)
	{
		pointMasses[i] = PointMass::create();
		pointMasses[i]->setMass(3.0f);
		pointMasses[i]->setMinDistance(0.05f);
		particleGroup->addModifier(pointMasses[i]);
	}

	// System
	particleSystem = System::create();
	particleSystem->addGroup(massGroup);
	particleSystem->addGroup(particleGroup);

	// setup some useful variables
	curtime=(f32)timer->getTime() / 1000.0f;

	SPKFactory::getInstance().traceAll();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin(true,true,COLOR_DEFAULT);

	oldtime = curtime;
	curtime = (f32)timer->getTime() / 1000.0f;
	deltaTime = curtime - oldtime;

	// Changes the color of the model over time
	step += deltaTime * 0.0002f;
	Vector3D color(0.6f + 0.4f * sin(step),0.6f + 0.4f * sin(step + PI * 2.0f / 3.0f),0.6f + 0.4f * sin(step + PI * 4.0f / 3.0f));
	particleModel->setParam(PARAM_RED,max(0.0f,color.x - 0.25f),min(1.0f,color.x + 0.25f));
	particleModel->setParam(PARAM_GREEN,max(0.0f,color.y - 0.25f),min(1.0f,color.y + 0.25f));
	particleModel->setParam(PARAM_BLUE,max(0.0f,color.z - 0.25f),min(1.0f,color.z + 0.25f));

	// If the number of particles in the mass group is 0 (at init or when the system is reinitialized),
	// a number of particles equal to NB_POINT_MASS is added
	//if (massGroup->getNbParticles() == 0) compile error
	static bool first=true;
	if(first)
	{
		for (int i = 0; i < NB_POINT_MASS; ++i)
			massGroup->addParticles(1,
			Vector3D(2.0f,0.0f,0.0f),
			Vector3D(randomizer->rand(-100,0)/100.f,randomizer->rand(-50,50)/100.f,randomizer->rand(-50,50)/100.f));
		massGroup->flushAddedParticles(); // to ensure particles are added
		first=false;
	}

	// Updates particle system
	particleSystem->update(deltaTime); // 1 defined as a second

	// the point masses are attached one by one to the particles in the massGroup so that they follow their moves
	for (int i = 0; i < NB_POINT_MASS; ++i)
		pointMasses[i]->setPosition(massGroup->getParticle(i).position());

	sceneMgr->render(videoDriver);

	particleSystem->render();
	
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