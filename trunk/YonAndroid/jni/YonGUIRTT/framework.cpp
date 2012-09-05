#include "framework.h"
#include "MyGUIRTTexture.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
//IGUIEnvirenment* guiEnv=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ICamera* pCamera2=NULL;
ILogger* logger=NULL;

MyGUI::MyGUIAdapter* guiAdapter;

IModel* cubeModel=NULL;
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
f32 factor=1.1f;

//ITexture* rtt=NULL;
MyGUI::Canvas* canvas=NULL;
MyGUI::IRenderTarget* rt=NULL;

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
				return MyGUI::InputManager::getInstance().injectMousePress(evt.mouseInput.x, evt.mouseInput.y, MyGUI::MouseButton::Left);
				//return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return MyGUI::InputManager::getInstance().injectMouseRelease(evt.mouseInput.x, evt.mouseInput.y, MyGUI::MouseButton::Left);
				//return true;
			case event::ENUM_MOUSE_INPUT_TYPE_MOVE:
				//logger->debug("[LM]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return MyGUI::InputManager::getInstance().injectMouseMove(evt.mouseInput.x, evt.mouseInput.y, 0);
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
				guiAdapter->onResize(core::dimension2du(evt.systemInput.screenWidth,evt.systemInput.screenHeight));
				return true;
			}
			break;
		}
		return false;
	}
};

void eventPreTextureChanges(MyGUI::Canvas* _canvas)
{
	Logger->debug("eventPreTextureChanges\r\n");
}
void requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
{
	Logger->debug("requestUpdateCanvas:%d,%d\r\n",_canvas->getWidth(),_canvas->getHeight());
	//MyGUI::IRenderTarget* rt=_canvas->getTexture()->getRenderTarget();
}
void frameEntered(float _time)
{
	Logger->debug("frameEntered\r\n");
}

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
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	guiAdapter=MyGUI::createMyGUIAdapter(fs,videoDriver,engine->getTimer(),geometryFty);

	const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();

	MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, size.height - 10 - 230, 300, 230), MyGUI::Align::Default, "Overlapped");
	window->setCaption("Camera view");
	window->setMinSize(MyGUI::IntSize(100, 100));
	canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

	//创建RTT纹理
	canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL, MyGUI::TextureUsage::RenderTarget);
	canvas->eventPreTextureChanges += MyGUI::newDelegate(eventPreTextureChanges);
	canvas->requestUpdateCanvas = MyGUI::newDelegate(requestUpdateCanvas);
	//canvas->updateTexture();

	//MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(frameEntered);
	//MyGUI::MyGUIRTTexture* rt=static_cast<MyGUI::MyGUIRTTexture*>(canvas->getTexture()->getRenderTarget());
	//rtt=rt->getTexture();

	//RT在Canvas resize时会被释放
	//rt=canvas->getTexture()->getRenderTarget();
	//Logger->debug("canvas:%d,%d\r\n",canvas->getWidth(),canvas->getHeight());

	//canvas->getWidth();
	//canvas->getHeight();
	//pCamera2=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	//pCamera2->set

	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));
 
	const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	core::rectf r(0,1,1,0);

	pCamera->render(videoDriver);

	rt=canvas->getTexture()->getRenderTarget();

	rt->begin();
	//Logger->debug("%d,%d\r\n",videoDriver->getCurrentRenderTargetSize().w,videoDriver->getCurrentRenderTargetSize().h);
	//teapotModel->setVisible(true);
	//sceneMgr->render(videoDriver);
	//teapotModel->setVisible(false);
	gfAdapter->clearZ(-1000);
	
	gfAdapter->drawRegion("trans.png",r,0,0,512,256,ENUM_TRANS_NONE);
	gfAdapter->render();

	rt->end();

	//sceneMgr->render(videoDriver);

	//Logger->debug("%d,%d\r\n",videoDriver->getCurrentRenderTargetSize().w,videoDriver->getCurrentRenderTargetSize().h);
	gfAdapter->clearZ(-1000);
	gfAdapter->drawRegion("trans.png",r,0,0,800,480,ENUM_TRANS_NONE);
	gfAdapter->render();

	//pCamera->render(videoDriver);
	guiAdapter->render();

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	delete guiAdapter;
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}