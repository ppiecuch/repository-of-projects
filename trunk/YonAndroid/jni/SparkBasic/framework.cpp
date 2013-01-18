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

System* particleSystem = NULL;
// setup some useful variables
float time,oldtime,deltaTime;
float step = 0.0f;

Model* particleModel=NULL;

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

	Vector3D gravity(0.0f,-0.8f,0.0f);

	YONQuadRenderer* particleRenderer = YONQuadRenderer::create(engine);
	particleRenderer->setTexture(videoDriver->getTexture("point.png"));
	particleRenderer->setTexturingMode(TEXTURE_2D);
	particleRenderer->setScale(0.05f,0.05f);
	particleRenderer->setBlending(BLENDING_ADD);
	particleRenderer->enableRenderingHint(DEPTH_WRITE,false);

	// Model
	particleModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA);
	particleModel->setParam(PARAM_ALPHA,0.8f); // constant alpha
	particleModel->setLifeTime(8.0f,8.0f);

	// Emitter
	SphericEmitter* particleEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.1f * PI,0.1f * PI);
	particleEmitter->setZone(Point::create(Vector3D(0.0f,0.02f,0.0f)));
	particleEmitter->setFlow(250);
	particleEmitter->setForce(1.5f,1.5f);

	// Obstacle
	Plane* groundPlane = Plane::create();
	Obstacle* obstacle = Obstacle::create(groundPlane,INTERSECT_ZONE,0.6f,1.0f);

	// Group
	Group* particleGroup = Group::create(particleModel,2100);
	particleGroup->addEmitter(particleEmitter);
	particleGroup->setRenderer(particleRenderer);
	particleGroup->addModifier(obstacle);
	particleGroup->setGravity(gravity);
	particleGroup->enableAABBComputing(false);

	particleSystem = YONSystem::create(sceneMgr->getRootSceneNode(),sceneMgr);
	particleSystem->addGroup(particleGroup);
	particleSystem->enableAABBComputing(false);

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

	// Changes the color of the model over time
	step += deltaTime * 0.5f;
	particleModel->setParam(PARAM_RED,0.6f + 0.4f * sin(step));
	particleModel->setParam(PARAM_GREEN,0.6f + 0.4f * sin(step + PI * 2.0f / 3.0f));
	particleModel->setParam(PARAM_BLUE,0.6f + 0.4f * sin(step + PI * 4.0f / 3.0f));

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