#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pOCamera=NULL;
ICamera* pPCamera=NULL;
ILogger* logger=NULL;
IRandomizer* randomizer=NULL;
ITimer* timer=NULL;

IModel* tCubeModel1=NULL;
IModel* tCubeModel2=NULL;
IModel* sCubeModel=NULL;
IModel* weedModel=NULL;


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
	params.fpsLimit=60;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pOCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300)); 
	pPCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_PERSP,core::vector3df(0,0,300)); 
	logger=Logger;
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif


	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	sCubeModel=sceneMgr->addModel(entity);
	material=sCubeModel->getMaterial(0);
	sCubeModel->setPosition(core::vector3df(100,-150,-800));
	material->setTexture(0,videoDriver->getTexture("test.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	tCubeModel1=sceneMgr->addModel(entity);
	material=tCubeModel1->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
	tCubeModel1->setPosition(core::vector3df(100,-150,-300));
	material->setTexture(0,videoDriver->getTexture("shadow.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	tCubeModel2=sceneMgr->addModel(entity);
	material=tCubeModel2->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
	tCubeModel2->setPosition(core::vector3df(100,-150,-350));
	material->setTexture(0,videoDriver->getTexture("shadow.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createWeed(200);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	weedModel=sceneMgr->addModel(entity);
	material=weedModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	material->states.CullFace=false;
	material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
	weedModel->setPosition(core::vector3df(150,-200,-500));
	material->setTexture(0,videoDriver->getTexture("plant00.png"));
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

	const core::vector3df tcrot1=tCubeModel1->getRotation();
	tCubeModel1->setRotation(core::vector3df(tcrot1.x,tcrot1.y+0.5f ,tcrot1.z));

	const core::vector3df tcrot2=tCubeModel2->getRotation();
	tCubeModel2->setRotation(core::vector3df(tcrot2.x,tcrot2.y-0.4f ,tcrot2.z));

	const core::vector3df wrot=weedModel->getRotation();
	weedModel->setRotation(core::vector3df(wrot.x,wrot.y+0.2f ,wrot.z));

	const core::vector3df scrot=sCubeModel->getRotation();
	sCubeModel->setRotation(core::vector3df(scrot.x,scrot.y-0.3f ,scrot.z));

	sceneMgr->setActiveCamera(pPCamera);
	sceneMgr->render(videoDriver);

	sceneMgr->setActiveCamera(pOCamera);
	pOCamera->render(videoDriver);
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