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
//IRandomizer* randomizer=NULL;
ITimer* timer=NULL;

float angleY = 0.0f;
float angleX = 12.0f;
float rainRatio = 0.5f;

// setup some useful variables
float curtime,oldtime,deltaTime;
float step = 0.0f;

Group* rainGroup = NULL;
Group* dropGroup = NULL;
Group* splashGroup = NULL;
System* particleSystem = NULL;
SphericEmitter* rainEmitter = NULL;
Emitter* dropEmitter = NULL;
Ring* rainZone = NULL;
Model* rainModel = NULL;
Model* dropModel = NULL;
Model* splashModel = NULL;
YONPointRenderer* dropRenderer = NULL;
YONLineRenderer* rainRenderer = NULL;

bool recompute = true;
float sizeRatio = 0;

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
				rainRatio += deltaTime;
				rainRatio = core::min_(1.0f,rainRatio);
				recompute=true;
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				YON_DEBUG("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_RDOWN:
				YON_DEBUG("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				rainRatio -= deltaTime;
				rainRatio = core::max_(0.0f,rainRatio);
				recompute=true;
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_RUP:
				YON_DEBUG("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
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

// Gets the values of a param function of the rain intensity
template<class T>
T param(T min,T max)
{
	return static_cast<T>(min + rainRatio * (max - min));
}

// Call back function to destroy rain particles when it touches the ground and create splash
bool killRain(Particle& particle,float deltaTime)
{
	if (particle.position().y <= 0.0f)
	{
		particle.position().set(particle.position().x,0.01f,particle.position().z);
		splashGroup->addParticles(1,particle.position(),Vector3D());
		dropGroup->addParticles(param(2,8),particle.position(),dropEmitter);
		return true;
	}
	return false;
}

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
	//randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/spark");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/spark");
#endif

	// Sets the update step
	System::setClampStep(true,0.1f);			// clamp the step to 100 ms
	System::useAdaptiveStep(0.001f,0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)

	Vector3D gravity(0.0f,-0.8f,0.0f);

	// Renderers
	// the size ratio is used with renderers whose size are defined in pixels. This is to adapt to any resolution
	sizeRatio = static_cast<float>(width) / 1440;

	YONPointRenderer* basicRenderer = YONPointRenderer::create(engine);

	// point renderer
	dropRenderer = YONPointRenderer::create(engine);
	dropRenderer->setType(POINT_CIRCLE);
	dropRenderer->setSize(2.0f * sizeRatio);
	//dropRenderer->enableBlending(true);

	// line renderer
	rainRenderer = YONLineRenderer::create(engine);
	rainRenderer->setLength(-0.1f);
	rainRenderer->setBlending(BLENDING_ALPHA);
	rainRenderer->enableRenderingHint(ALPHA_TEST,true); // uses the alpha test
	//rainRenderer->enableBlending(true);

	// quad renderer
	YONQuadRenderer* splashRenderer = YONQuadRenderer::create(engine);
	splashRenderer->setScale(0.05f,0.05f);
	splashRenderer->setTexturingMode(TEXTURE_2D);
	splashRenderer->setTexture(videoDriver->getTexture("waterdrops.png"));
	splashRenderer->setBlending(BLENDING_ALPHA);
	splashRenderer->enableRenderingHint(ALPHA_TEST,true); // uses the alpha test
	//splashRenderer->enableBlending(true);
	splashRenderer->enableRenderingHint(DEPTH_WRITE,false);

	// Models
	// rain model
	rainModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_MASS,
		0,
		FLAG_MASS);

	rainModel->setParam(PARAM_ALPHA,0.2f);
	rainModel->setImmortal(true);

	// drop model
	dropModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_MASS,
		0,
		FLAG_MASS);

	dropModel->setParam(PARAM_ALPHA,0.6f);

	// splash model
	splashModel = Model::create(FLAG_GREEN | FLAG_RED | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE,
		FLAG_SIZE | FLAG_ALPHA,
		FLAG_SIZE | FLAG_ANGLE);

	splashModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI);
	splashModel->setParam(PARAM_ALPHA,1.0f,0.0f);

	// rain emitter
	//AABox* rainZone = AABox::create(Vector3D(0.0f,5.0f,0.0f));
	rainZone = Ring::create(Vector3D(0.0f,5.0f,0.0f));
	rainEmitter = SphericEmitter::create(Vector3D(0.0f,-1.0f,0.0f),0.0f,0.03f * PI);
	rainEmitter->setZone(rainZone);

	// drop emitter
	dropEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.0f,0.2f * PI);

	// Groups
	// rain group
	rainGroup = Group::create(rainModel,8000);
	rainGroup->setCustomUpdate(&killRain);
	rainGroup->setRenderer(rainRenderer);
	rainGroup->addEmitter(rainEmitter);
	rainGroup->setFriction(0.7f);
	rainGroup->setGravity(gravity);

	// drop group
	dropGroup = Group::create(dropModel,16000);
	dropGroup->setRenderer(dropRenderer);
	dropGroup->setFriction(0.7f);
	dropGroup->setGravity(gravity);

	// splash group
	splashGroup = Group::create(splashModel,2400);
	splashGroup->setRenderer(splashRenderer);

	// System
	particleSystem = System::create();
	particleSystem->addGroup(splashGroup);
	particleSystem->addGroup(dropGroup);
	particleSystem->addGroup(rainGroup);


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

	rainZone->setPosition(Vector3D(pCamera->getAbsolutePosition().x,5.0f,pCamera->getAbsolutePosition().z));
	// if the ratio rate has changed, recompute the particule system parameters
	if (recompute)
	{
		rainModel->setParam(PARAM_RED,param(1.0f,0.40f));
		rainModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
		rainModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
		rainModel->setParam(PARAM_MASS,param(0.4f,0.8f),param(0.8f,1.6f));

		dropModel->setParam(PARAM_RED,param(1.0f,0.40f));
		dropModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
		dropModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
		dropModel->setParam(PARAM_MASS,param(0.4f,0.8f),param(3.0f,4.0f));
		dropModel->setLifeTime(param(0.05f,0.3f),param(0.1f,0.5f));

		splashModel->setParam(PARAM_RED,param(1.0f,0.40f));
		splashModel->setParam(PARAM_GREEN,param(1.0f,0.40f));
		splashModel->setParam(PARAM_BLUE,param(1.0f,0.42f));
		splashModel->setParam(PARAM_SIZE,0.0f,0.0f,param(0.375f,2.25f),param(0.75f,3.78f));
		splashModel->setLifeTime(param(0.2f,0.3f),param(0.4f,0.5f));

		rainEmitter->setFlow(param(0.0f,4800.0f));
		rainEmitter->setForce(param(3.0f,5.0f),param(6.0f,10.0f));
		rainZone->setRadius(0.0f,param(20.0f,5.0f));
		//rainZone->setDimension(Vector3D(param(40.0f,10.0f),0.0f,param(40.0f,10.0f)));

		dropEmitter->setForce(param(0.1f,1.0f),param(0.2f,2.0f));

		dropRenderer->setSize(param(1.0f,3.0f) * sizeRatio);
		rainRenderer->setWidth(param(1.0f,4.0f) * sizeRatio);
		recompute = false;
	}
	// Updates particle system
	particleSystem->update(deltaTime); // 1 defined as a second

	sceneMgr->render(videoDriver);

	// Renders particle system
	particleSystem->render();
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,RAINRATE:%d,TRI:%d,PTL:%d,DWL:%d",videoDriver->getFPS(),(int)(rainRatio * 100.0f),videoDriver->getPrimitiveCountDrawn(),particleSystem->getNbParticles(),videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

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