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
ITimer* timer=NULL;
ITextSystem* textSystem=NULL;

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
IFontFamily* family1=NULL;
IFontFamily* family2=NULL;
IFontFamily* family3=NULL;
IFontFamily* family4=NULL;
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
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();
	textSystem=engine->getTextSystem();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	family1=textSystem->getFontFamily("staaiti.ttf");
	textSystem->addText(core::ustring(L"天行健，君子以自强不息，地势坤，君子以厚德载物"),family1,core::position2di(-200,-50),20,video::COLOR_GREEN, text::ENUM_FONT_RENDER_MODE_NORMAL);

	family2=textSystem->getFontFamily("mingliu.ttc");
	textSystem->addText(core::ustring(L"莫为浮云遮望眼，风物长宜放眼量"),family2,core::position2di(-200,-70),11,video::COLOR_RED);

	family3=textSystem->getFontFamily("simsun.ttc");
	textSystem->addText(core::ustring(L"中军账下美人靠，怀拥将军度良宵"),family3,core::position2di(-200,-90),13,video::COLOR_BLUE);

	family4=textSystem->getFontFamily("stxingka.ttf");
	textSystem->addText(core::ustring(L"人生结交在终始，莫为升沉中路分"),family4,core::position2di(-200,-110),18 ,video::COLOR_PURPLE, text::ENUM_FONT_RENDER_MODE_NORMAL);

	return true;
}
ITexture* texture1=NULL;
ITexture* texture2=NULL;
ITexture* texture3=NULL;
ITexture* texture4=NULL;
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	textSystem->render(videoDriver);

	/*if(family1!=NULL&&texture1==NULL)
	{
		core::stringc name("FT2FontPage_%s",core::getFileName(family1->getPath()).c_str());
		texture1=videoDriver->getTexture(name);
	}
	if(family2!=NULL&&texture2==NULL)
	{
		core::stringc name("FT2FontPage_%s",core::getFileName(family2->getPath()).c_str());
		texture2=videoDriver->getTexture(name);
	}
	if(family4!=NULL&&texture4==NULL)
	{
		core::stringc name("FT2FontPage_%s",core::getFileName(family4->getPath()).c_str());
		texture4=videoDriver->getTexture(name);
	}*/
	

	gfAdapter->clearZ(-1000);
	//gfAdapter->drawImage(texture1,10,10,true);
	//gfAdapter->drawImage(texture2,400,10,true);
	gfAdapter->drawImage(texture4,400,10,true);
	gfAdapter->render();
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,DCL:%d,TCC:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall(),videoDriver->getTextureChange()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}