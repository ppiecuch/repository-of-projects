
#include "framework.h"
#include "MyGUIRTTexture.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
//IGUIEnvirenment* guiEnv=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IGraphicsAdapter* gfAdapterWindow=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ICamera* pCamera2=NULL;
ILogger* logger=NULL;

MyGUI::MyGUIAdapter* guiAdapter;

IModel* cubeModel=NULL;
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
f32 factor=1.1f;

core::matrix4f projection;
core::matrix4f oldProjection;
core::dimension2di canvasDim;

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
			case event::ENUM_MOUSE_INPUT_TYPE_MDOWN:
				logger->debug("[MP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_DOZE;
					getEngine()->postEventFromUser(evt);
				}
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_MUP:
				logger->debug("[MR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_WAKE;
					getEngine()->postEventFromUser(evt);
				}
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
				guiAdapter->onResize(core::dimension2du(evt.systemInput.screenWidth,evt.systemInput.screenHeight));
				return true;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				{
					//有了setEventReceivable,不再需要调用updateTexture
					//canvas->updateTexture();
				}
			}
			break;
		}
		return false;
	}
};


//canvas调整大小时，尺寸超过贴图尺寸会触发重建纹理流程，重建前会调用此函数
void eventPreTextureChanges(MyGUI::Canvas* _canvas)
{
	Logger->debug("eventPreTextureChanges\r\n");
}
void requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
{
	Logger->debug("requestUpdateCanvas:%d,%d\r\n",_canvas->getWidth(),_canvas->getHeight());
	//MyGUI::IRenderTarget* rt=_canvas->getTexture()->getRenderTarget();
	f32 hh=_canvas->getHeight()/2;
	f32 hw=_canvas->getWidth()/2;
	projection.makeIdentity();
	projection.ortho(-hw,hw,-hh,hh,1,3000);

	canvasDim.w=_canvas->getWidth();
	canvasDim.h=_canvas->getHeight();

	pCamera2->onResize(core::dimension2du(_canvas->getWidth(),_canvas->getHeight()));
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
	gfAdapter=engine->getGraphicsAdapterWindow();
	gfAdapterWindow=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera2=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	pCamera2->setEventReceivable(false);
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0));
	//pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	guiAdapter=MyGUI::createMyGUIAdapter(fs,videoDriver,engine->getTimer(),geometryFty);

	const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
	root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Sample colour picker implementation. Select text in EditBox and then select colour to colour selected part of text.");

	const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();

	
	MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, size.height - 10 - 230, 300, 230), MyGUI::Align::Default, "Overlapped");
	window->setCaption("Camera view");
	window->setMinSize(MyGUI::IntSize(100, 100));
	canvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);

	//创建RTT纹理
	//TODO,使用TRM_PT_VIEW_ALL在resize时会有边缘错乱的情况
	//canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL, MyGUI::TextureUsage::RenderTarget);
	canvas->createTexture(MyGUI::Canvas::TRM_PT_CONST_SIZE, MyGUI::TextureUsage::RenderTarget);
	canvas->eventPreTextureChanges += MyGUI::newDelegate(eventPreTextureChanges);
	canvas->requestUpdateCanvas = MyGUI::newDelegate(requestUpdateCanvas);
	canvas->updateTexture();
	

	//MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(frameEntered);
	//MyGUI::MyGUIRTTexture* rt=static_cast<MyGUI::MyGUIRTTexture*>(canvas->getTexture()->getRenderTarget());
	//rtt=rt->getTexture();

	//RT在Canvas resize时会被释放
	//rt=canvas->getTexture()->getRenderTarget();
	//Logger->debug("canvas:%d,%d\r\n",canvas->getWidth(),canvas->getHeight());

	//canvas->getWidth();
	//canvas->getHeight();
	//pCamera2=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	//sceneMgr->setViewingCamera(pCamera);

	IShap *shap;
	IUnit* unit;
	IEntity* entity;
	//IMaterial* material;

	/*shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(0,0,0));
	MyGUI::MyGUIRTTexture* rt=static_cast<MyGUI::MyGUIRTTexture*>(canvas->getTexture()->getRenderTarget());
	material->setTexture(0,rt->getTexture());
	shap->drop();
	unit->drop();
	entity->drop();*/

	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=cubeModel->getMaterial(0);
#if 0
		material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
		//material.MaterialType=ENUM_MATERIAL_TYPE_TRANSPARENT;
		material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
		material.BlendDst=ENUM_BLEND_FACTOR_ONE;
#endif
		material.setTexture(0,videoDriver->getTexture("test.png"));
	}
	cubeModel->setPosition(core::vector3df(100,100,0)); 
	shap->drop();
	unit->drop();
	entity->drop();

	/*shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=planeModel->getMaterial(0);
		material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
		material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
		material.BlendDst=ENUM_BLEND_FACTOR_ONE;
		material.setTexture(0,videoDriver->getTexture("aura.png"));
	}
	planeModel->setPosition(core::vector3df(0,0,0));
	shap->drop();
	unit->drop();
	entity->drop();*/

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));
 
	//const core::vector3df trot=teapotModel->getRotation();
	//teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	core::rectf r(0,1,1,0);
	rt=canvas->getTexture()->getRenderTarget();

	//oldProjection=videoDriver->getTransform(ENUM_TRANSFORM_PROJECTION);
	//videoDriver->setTransform(ENUM_TRANSFORM_PROJECTION,projection);
	pCamera2->setNeedUpload();
	pCamera2->render(videoDriver);
	rt->begin();
	//Logger->debug("%d,%d\r\n",videoDriver->getCurrentRenderTargetSize().w,videoDriver->getCurrentRenderTargetSize().h);
	//teapotModel->setVisible(true);
	//sceneMgr->render(videoDriver);
	//teapotModel->setVisible(false);
	gfAdapter->clearZ(1000);
	
	gfAdapter->drawRegion("trans.png",r,0,0,128,64,ENUM_TRANS_NONE);
	gfAdapter->drawRegion("test-png8.png",r,128,64,64,64,ENUM_TRANS_MIRROR);
	gfAdapter->render();

	rt->end();
	//videoDriver->setTransform(ENUM_TRANSFORM_PROJECTION,oldProjection);

	sceneMgr->render(videoDriver);
	//pCamera->setNeedUpload();
	//pCamera->render(videoDriver);

	//Logger->debug("%d,%d\r\n",videoDriver->getCurrentRenderTargetSize().w,videoDriver->getCurrentRenderTargetSize().h);
	//gfAdapterWindow->clearZ(1000);
	//gfAdapterWindow->drawRegion("trans.png",r,180,200,128,64,ENUM_TRANS_NONE);
	//gfAdapterWindow->render();

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
