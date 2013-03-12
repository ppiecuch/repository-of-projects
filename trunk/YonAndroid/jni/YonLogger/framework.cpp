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
//IRandomizer* randomizer=NULL;
ITimer* timer=NULL;

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(evt.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_DOZE:
				logger->debug("DOZE\r\n");
				break;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				logger->debug("WAKE\r\n");
				break;
			}
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
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300)); 
	logger=Logger;
	//randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	/*Logger->debug("debug\r\n");
	Logger->info("info\r\n");
	Logger->warn("warn\r\n");
	Logger->error("error\r\n");*/

	//YON_FAIL_IF(true);
	/*ITexture* tex=videoDriver->getTexture("test.png");
	videoDriver->grabTexture(tex);
	videoDriver->grabTexture(tex);

	videoDriver->dropTexture(tex);
	videoDriver->dropTexture(tex);

	ITexture* tex2=videoDriver->getTexture("de.png");
	videoDriver->grabTexture(tex2);

	videoDriver->dropTexture(tex2);*/
	//videoDriver->dropTexture(tex2);

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	//PROFILE_REGISTER_FRAME();
	videoDriver->begin(true,true,COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	//Logger->drawString(videoDriver,core::stringc("ABCDEFGHIJKLMNOPQRSTUVWXYZ"),core::position2di(0,20),COLOR_GREEN);
	//Logger->drawString(videoDriver,core::stringc("abcdefghijklmnopqrstuvwxyz"),core::position2di(0,40),COLOR_GREEN);
	//Logger->drawString(videoDriver,core::stringc("0123456789+-*/\,./?<>|="),core::position2di(0,60),COLOR_GREEN);
	
	//TODO 此句一开，Android上必崩
	//Logger->render(videoDriver);

	u32 start=timer->getRealTime();
	u32 end=timer->getRealTime();
	Logger->drawString(videoDriver,core::stringc("use time:%d",(end-start)),core::position2di(200,0),COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	//PROFILE_REPORT();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}