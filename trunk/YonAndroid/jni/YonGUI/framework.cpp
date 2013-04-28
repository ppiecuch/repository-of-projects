#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGUISystem* guiSystem=NULL;
IGraphicsAdapter* gfAdapter=NULL;
ITextSystem* textSystem=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
II18NManager* i18nMgr=NULL;

IButton* button=NULL;


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
			case event::ENUM_MOUSE_INPUT_TYPE_MOVE:
				//logger->debug("[LM]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
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
				return true;
			}
			break;
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(evt.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_RESIZE:
				return true;
			}
			break;
		}
		return false;
	}
};

bool init(void *pJNIEnv,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	guiSystem=engine->getGUISystem();
	gfAdapter=engine->getGraphicsAdapter();
	textSystem=engine->getTextSystem();
	i18nMgr=engine->getI18NManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	
#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/ui");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/ui");
#endif

	//IFontFamily* fontFamily=textSystem->getFontFamily("simsun.ttc");
	IFontFamily* fontFamily=textSystem->getFontFamily("Droid Sans Fallback.ttf");
	guiSystem->setDefaultFontFamily(fontFamily);

	ITheme* theme=guiSystem->getBindedTheme();
	gui::SSkin skin;

	ITexture* texture=videoDriver->getTexture("skin00.png");

	skin.Slices[widget::WST].build(texture,78,629,134-78,682-629);
	skin.Slices[widget::CNT].build(texture,150,629,151-150,682-629);
	skin.Slices[widget::EST].build(texture,166,629,223-166,682-629);
	skin.Place=widget::HORIZONTAL;
	skin.Type=widget::BUTTON;
	theme->addSkin(widget::BUTTON,widget::HOVER,skin);

	skin.Slices[widget::WST].build(texture,78,704,136-78,757-704);
	skin.Slices[widget::CNT].build(texture,150,704,151-150,757-704);
	skin.Slices[widget::EST].build(texture,166,704,223-166,757-704);
	skin.Place=widget::HORIZONTAL;
	skin.Type=widget::BUTTON;
	theme->addSkin(widget::BUTTON,widget::LEAVE,skin);

	skin.Slices[widget::WST].build(texture,78,861,136-78,914-861);
	skin.Slices[widget::CNT].build(texture,150,861,151-150,914-861);
	skin.Slices[widget::EST].build(texture,166,861,223-166,914-861);
	skin.Place=widget::HORIZONTAL;
	skin.Type=widget::BUTTON;
	theme->addSkin(widget::BUTTON,widget::DOWN,skin);

	button=guiSystem->addButton(NULL,"test",core::recti(10,240,10+223-78,240+682-629));
	core::ustring str=i18nMgr->convert("²âÊÔa²âÊÔ",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	//button->getText()->setText(str);
	button->setText(str);

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin();

	guiSystem->render();

	textSystem->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%u,DCL:%u",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();

	;
}
void destroy(){
	//delete guiAdapter;
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}