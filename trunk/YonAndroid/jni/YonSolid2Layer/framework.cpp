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

ISceneNode* planeModel=NULL;
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
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif


	IShap *shap;
	IUnit* unit;
	IEntity* entity;

#if 1
	shap=geometryFty->createXYRectangle2D2T(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addSceneNode(entity);
	{
		SMaterial& material=planeModel->getMaterial(0);
		material.MaterialType=ENUM_MATERIAL_TYPE_SOLID_2_LAYER;
		material.setTexture(0,videoDriver->getTexture("aura_rgb.png"));
		material.setTexture(1,videoDriver->getTexture("aura_mask.png"));
	}
	planeModel->setPosition(core::vector3df(0,0,0));
	
	shap->drop();
	unit->drop();
	entity->drop();

#else

	shap=geometryFty->createXYRectangle2D(-125,-25,-75,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=planeModel->getMaterial(0);
		material.setTexture(0,videoDriver->getTexture("skill/skill0.png"));
	}
	planeModel->setPosition(core::vector3df(0,0,0));

	shap->drop();
	unit->drop();
	entity->drop();

	//shap=geometryFty->createXYRectangle2D2T(-125,-25,-75,25,0,0,1,1,0,0,0.25f,1);
	//shap=geometryFty->createXYRectangle2D2T(-125,-25,-75,25,0,0,1,1,0.25f,0,0.5f,1);
	//shap=geometryFty->createXYRectangle2D2T(-125,-25,-75,25,0,0,1,1,0.5f,0,0.75f,1);
	shap=geometryFty->createXYRectangle2D2T(-125,-25,-75,25,0,0,1,1,0.75f,0,1,1);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	IModel* waterfallModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=waterfallModel->getMaterial(0);
		material.MaterialType=ENUM_MATERIAL_TYPE_MASK;
		material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
		material.BlendDst=ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		material.setTexture(0,videoDriver->getTexture("skill/gray.png"));
		material.setTexture(1,videoDriver->getTexture("skill/mask.png"));
	}
	shap->drop();
	unit->drop();
	entity->drop();

#endif

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);
	/*

	const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.9f;
	else if(psca.x<2)
		factor=1.1f;
	planeModel->setScale(psca*factor);*/

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}