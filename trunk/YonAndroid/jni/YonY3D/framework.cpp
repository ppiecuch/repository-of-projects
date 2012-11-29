#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pOverlookCamera=NULL;
ICamera* pOrthoCamera=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
IRandomizer* randomizer=NULL;

IModel* planeModel=NULL;
IModel* teapotModel=NULL;
f32 factor=1.1f;

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCameraFPS();
	pCamera->setFar(5000);
	pCamera->setNear(30);
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
	fs->addWorkingDirectory("../media/xc3d",true);
	//fs->addWorkingDirectory("../media/terrain/heightmap/plain");
	fs->addWorkingDirectory("../media/terrain/heightmap/rough");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
#endif

	IAnimatedEntity* entity=sceneMgr->getEntity("miner.y3d");
	IAnimatedSceneNode* node=sceneMgr->addAnimatedSceneNode(entity);
	//临时需要drop，添加EntityCache后就不用了
	entity->drop();

	
	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}