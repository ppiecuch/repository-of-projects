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
IGeometryFactory* geometryFty=NULL;

bool needAdd=false;
s32 textureSize=0;
IModel* planeModel;
f32 factor=1.1f;

f32 xx0,yy0,xx1,yy1;

core::stringc ramAvail="0";
core::stringc ramTotal="0";
core::position2di vrampos=core::position2di(0,36);
core::position2di rampos=core::position2di(0,24);
core::position2di texpos=core::position2di(0,12);
void setRAMAvail(const c8* str)
{
	ramAvail=str;
}
void setRAMTotal(const c8* str)
{
	ramTotal=str;
}

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
				needAdd=true;
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
					//SCallback cb;
					//cb.type=platform::ENUM_CALLBACK_TYPE_UI;
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
					//cb.ui.type=platform::ENUM_CALLBACK_UI_TYPE_EDITBOX;
					//engine->callback(cb);
					

					needAdd=true;

					SCallback cb;
					cb.type=ENUM_CALLBACK_TYPE_HW;
					cb.hw.type=ENUM_CALLBACK_HW_TYPE_RAM_AVAIL;
					engine->callback(cb);
					cb.hw.type=ENUM_CALLBACK_HW_TYPE_RAM_TOTAL;
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

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.pCallback=pcb;
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	//fs->setWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../Yon/");
	fs->setWorkingDirectory("../media/vram/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	//fs->setWorkingDirectory("media/");
	//fs->addWorkingDirectory("media/");
	//fs->addWorkingDirectory("media/vram/");
	//fs->addWorkingDirectory("temp/");
	fs->setWorkingDirectory("media/vram/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,true);


	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_REF);
	planeModel->setPosition(core::vector3df(0,0,0));
	material->setTexture(0,videoDriver->getTexture("0.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	xx0=-videoDriver->getCurrentRenderTargetSize().w/2;
	yy0=-videoDriver->getCurrentRenderTargetSize().h/2;
	xx1=videoDriver->getCurrentRenderTargetSize().w/2;
	yy1=videoDriver->getCurrentRenderTargetSize().h/2;
	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
	xx0=-videoDriver->getCurrentRenderTargetSize().w/2;
	yy0=-videoDriver->getCurrentRenderTargetSize().h/2;
	xx1=videoDriver->getCurrentRenderTargetSize().w/2;
	yy1=videoDriver->getCurrentRenderTargetSize().h/2;
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	if(needAdd)
	{
		IMaterial* material;
		IShap *shap;
		IUnit* unit;
		IEntity* entity;
		IModel* planeModel;

		

		shap=geometryFty->createXYRectangle2D(xx0,yy0,xx1,yy1);
		unit=geometryFty->createUnit(shap);
		entity=geometryFty->createEntity(unit);
		planeModel=sceneMgr->addModel(entity);
		material=planeModel->getMaterial(0);
		planeModel->setPosition(core::vector3df(0,0,textureSize));
		material->setTexture(0,videoDriver->getTexture(io::path("%d.png",textureSize)));
		shap->drop();
		unit->drop();
		entity->drop();

		++textureSize;

		needAdd=false;
	}

	const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.9f;
	else if(psca.x<2)
		factor=1.1f;
	planeModel->setScale(psca*factor);

	sceneMgr->render(videoDriver);

	//Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("TEX:%d",textureSize),texpos,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("RAM:%s/%s",ramAvail.c_str(),ramTotal.c_str()),rampos,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("VRAM:%d(%s)",videoDriver->getVideoMemory(),videoDriver->getVideoMemoryString()),vrampos,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}