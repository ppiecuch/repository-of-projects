#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
II18NManager* i18nManager=NULL;
ITimer* timer=NULL;

SMaterial pointMaterial;
SDynamicShap2D pointShap;

ISceneNode* cubeModel=NULL;
ISceneNode* weedModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
f32 factor=1.1f;

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt)
	{
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
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
				return true;
			}
		}
		return false;
	}
};
bool init(void *pJNIEnv,ICallback* pcb,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.pCallback=pcb;
	//params.fpsLimit=10;
	params.appPath=appPath;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	i18nManager=engine->getI18NManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	//fs->addWorkingDirectory("../media/",true);
	fs->addWorkingDirectory("../Yon/");
	//fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/",true);
#endif

	
	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}