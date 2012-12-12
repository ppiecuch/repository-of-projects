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
IRandomizer* randomizer=NULL;

bool show=false;
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

				//editbox
				SCallback cb;
				cb.type=platform::ENUM_CALLBACK_TYPE_UI;
				cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_EDITBOX;
				engine->callback(cb);
				return true;
			}
			break;
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				{
					SCallback cb;
					cb.type=platform::ENUM_CALLBACK_TYPE_UI;
					//confirm
					/*
					cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_CONFIRM;
					cb.ui.title="标题";
					cb.ui.content="content";
					cb.ui.positiveButton="ok";
					cb.ui.negativeButton="cancel";
					*/

					//toast
					//cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_TOAST;
					//cb.ui.content="你好Helloworld";

					//spinner
					/*
					if(show==false){
						cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_LOADING;
						cb.ui.content="请稍候wating";
						cb.ui.visible=true;
						show=true;
					}else{
						cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_LOADING;
						cb.ui.visible=false;
						show=false;
					}
					*/

					//editbox
					cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_EDITBOX;

					engine->callback(cb);
				}
				return true;
			}
			break;
		case ENUM_EVENT_TYPE_UI:
			switch(evt.uiInput.type)
			{
			case ENUM_UI_INPUT_TYPE_EDITTEXT:
				Logger->debug("edittext:%s\r\n",evt.uiInput.str);
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
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../Yon/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("temp/");
#endif


	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,DCL:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}