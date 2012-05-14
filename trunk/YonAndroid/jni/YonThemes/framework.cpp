#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGUIEnvirenment* guiEnv=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;

IModel* cubeModel=NULL;
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
f32 factor=1.1f;

MyGUI::Widget* mDemoView=NULL;
MyGUI::ComboBox* mComboSkins=NULL;
void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
void createDemo(size_t _index);
void destroyDemo();
void colourWidgets(MyGUI::Widget* _widget, const MyGUI::Colour& _colour);
void createDemo(size_t _index)
{
	destroyDemo();

	if (_index == 0)
	{
		MyGUI::ResourceManager::getInstance().load("MyGUI_BlueWhiteTheme.xml");
	}
	else if (_index == 1)
	{
		MyGUI::ResourceManager::getInstance().load("MyGUI_BlackBlueTheme.xml");
	}
	else if (_index == 2)
	{
		MyGUI::ResourceManager::getInstance().load("MyGUI_BlackOrangeTheme.xml");
	}
	/*else if (_index == 3)
	{
		MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_grayscale_tag.xml");
		MyGUI::ResourceManager::getInstance().load("core_skin.xml");
	}*/

	MyGUI::VectorWidgetPtr windows = MyGUI::LayoutManager::getInstance().loadLayout("Themes.layout");
	MYGUI_ASSERT(windows.size() == 1, "Error load layout");
	mDemoView = windows[0];
	if (_index == 3)
	{
		colourWidgets(mDemoView, MyGUI::Colour::Green);
	}

	mComboSkins = MyGUI::Gui::getInstance().findWidget<MyGUI::ComboBox>("Combo");
	mComboSkins->setComboModeDrop(true);
	mComboSkins->addItem("blue & white");
	mComboSkins->addItem("black & blue");
	mComboSkins->addItem("black & orange");
	//mComboSkins->addItem("gray");

	mComboSkins->setIndexSelected(_index);
	mComboSkins->eventComboAccept += MyGUI::newDelegate(notifyComboAccept);
}

void colourWidgets(MyGUI::Widget* _widget, const MyGUI::Colour& _colour)
{
	_widget->setColour(_colour);
	MyGUI::EnumeratorWidgetPtr enumerator = _widget->getEnumerator();
	while (enumerator.next())
	{
		colourWidgets(enumerator.current(), _colour);
	}
}

void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
{
	createDemo(_index);
}

void destroyDemo()
{
	if (mDemoView)
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mDemoView);
		mDemoView = 0;
	}
	if (mComboSkins)
	{
		//MyGUI::WidgetManager::getInstance().destroyWidget(mComboSkins);
		mComboSkins = 0;
	}
}


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
				logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,u32 width,u32 height){
	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	guiEnv=engine->getGUIEnvirentment();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(core::vector3df(0,0,300));
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

	guiEnv->init();
	

	createDemo(0);

	IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

	/*
	ISound* sound=audioDriver->getSound("bg.ogg");
	sound->setLooping(true);
	//sound->setGain(0.5f);
	sound->play();
	//sound=audioDriver->getSound("helloworld.wav");
	//sound->play();
	*/

	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	cubeModel->setPosition(core::vector3df(100,100,0)); 
	material->setTexture(0,videoDriver->getTexture("test.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
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
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(0,0,0));
	material->setTexture(0,videoDriver->getTexture("aura.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	//core::array<core::stringc> arr;
	//arr.push_back("test");

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	const core::vector3df crot=cubeModel->getRotation();
	cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.9f;
	else if(psca.x<2)
		factor=1.1f;
	planeModel->setScale(psca*factor);

	sceneMgr->render(videoDriver);
	
	//pCamera->render(videoDriver);
	guiEnv->render();

	

	Logger->drawString(core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();

	;
}
void destroy(){
	destroyDemo();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}