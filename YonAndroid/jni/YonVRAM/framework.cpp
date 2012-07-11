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
IModel* teapotModel=NULL;
f32 factor=1.1f;

f32 xx0,yy0,xx1,yy1;

core::stringc ramAvail="0";
core::stringc ramTotal="0";
core::stringc ramThreshold="0";
core::position2di vrampos=core::position2di(0,48);
core::position2di rampos=core::position2di(0,32);
core::position2di texpos=core::position2di(0,16);
void setRAMAvail(const c8* str)
{
	ramAvail=str;
}
void setRAMTotal(const c8* str)
{
	ramTotal=str;
}
void setRAMThreshold(const c8* str)
{
	ramThreshold=str;
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
	//params.fpsLimit=43;
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
	fs->setWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../Yon/");
	//fs->setWorkingDirectory("../media/vram/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	//fs->setWorkingDirectory("media/");
	//fs->addWorkingDirectory("media/");
	//fs->addWorkingDirectory("media/vram/");
	//fs->addWorkingDirectory("temp/");
	fs->setWorkingDirectory("media/vram/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,true);

	SCallback cb;
	cb.type=ENUM_CALLBACK_TYPE_HW;
	cb.hw.type=ENUM_CALLBACK_HW_TYPE_RAM_THRESHOLD;
	engine->callback(cb);


	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(50,-50,200));
	shap->drop();
	unit->drop();
	entity->drop();
	teapotModel->grab();
	sceneMgr->clearModels();



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

		

		for(s32 i=0;i<10;++i)
		{
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

		}
		needAdd=false;
	}

	const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	sceneMgr->render(videoDriver);

	teapotModel->render(videoDriver);

	//Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_RED);
	Logger->drawString(videoDriver,core::stringc("TEX:%d",textureSize),texpos,COLOR_RED);
	Logger->drawString(videoDriver,core::stringc("RAM:%s/%s(%s)",ramAvail.c_str(),ramTotal.c_str(),ramThreshold.c_str()),rampos,COLOR_RED);
	Logger->drawString(videoDriver,core::stringc("VRAM:%d(%s)",videoDriver->getVideoMemory(),videoDriver->getVideoMemoryString()),vrampos,COLOR_RED);

	videoDriver->end();
}
void destroy(){
	teapotModel->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}