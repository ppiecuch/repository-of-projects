#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGUISystem* guiSystem=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;

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
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/ui");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/ui");
#endif

	ITheme* theme=guiSystem->getBindedTheme();
	gui::SSkin skin;

	ITexture* texture=videoDriver->getTexture("skin00.png");

	skin.Slices[widget::WST].build(texture,78,629,136-78,682-629);
	skin.Slices[widget::CNT].build(texture,145,629,146-145,682-629);
	skin.Slices[widget::EST].build(texture,164,629,223-164,682-629);
	skin.Place=widget::HORIZONTAL;
	theme->addSkin(widget::BUTTON,widget::HOVER,widget::CNT,skin);

	skin.Slices[widget::WST].build(texture,78,704,136-78,757-704);
	skin.Slices[widget::CNT].build(texture,145,704,223-145,757-704);
	skin.Slices[widget::EST].build(texture,164,704,223-164,757-704);
	skin.Place=widget::HORIZONTAL;
	theme->addSkin(widget::BUTTON,widget::LEAVE,widget::CNT,skin);

	skin.Slices[widget::WST].build(texture,78,861,136-78,914-861);
	skin.Slices[widget::CNT].build(texture,145,861,223-145,914-861);
	skin.Slices[widget::EST].build(texture,164,861,223-164,914-861);
	skin.Place=widget::HORIZONTAL;
	theme->addSkin(widget::BUTTON,widget::DOWN,widget::CNT,skin);

	button=guiSystem->addButton(NULL,"test",core::recti(10,240,10+223-78,240+682-629));

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin();

	guiSystem->render();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%u",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

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