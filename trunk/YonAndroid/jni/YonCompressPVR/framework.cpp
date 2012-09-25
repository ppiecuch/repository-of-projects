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

IModel* cubeModel=NULL;
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
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_DOZE;
					getEngine()->postEventFromUser(evt);
				}
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_WAKE;
					getEngine()->postEventFromUser(evt);
				}
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
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	if(engine->available()==false)
		return false;
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
	fs->addWorkingDirectory("../media/compressTexture",true);
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/compressTexture",true);
#endif

	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	SAnimatorParam rotateAnimatorParam;
	rotateAnimatorParam.type=ENUM_ANIMATOR_TYPE_ROTATE;
	rotateAnimatorParam.animatorRotate.rotateSpeed=0.1f;
	IAnimator* rotateAnimator=animatorFty->createAnimator(rotateAnimatorParam);

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	cubeModel->setPosition(core::vector3df(-140,100,0));
	material->setTexture(0,videoDriver->getTexture("red.pvr"));
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_REF);
	shap->drop();
	unit->drop();
	entity->drop();

	cubeModel->addAnimator(rotateAnimator);

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	cubeModel->setPosition(core::vector3df(140,100,0));
	material->setTexture(0,videoDriver->getTexture("green.pvr"));
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_REF);
	shap->drop();
	unit->drop();
	entity->drop();

	cubeModel->addAnimator(rotateAnimator);

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	cubeModel->setPosition(core::vector3df(-140,-100,0));
	material->setTexture(0,videoDriver->getTexture("blue.pvr"));
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_REF);
	shap->drop();
	unit->drop();
	entity->drop();

	cubeModel->addAnimator(rotateAnimator);

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	cubeModel->setPosition(core::vector3df(140,-100,0));
	material->setTexture(0,videoDriver->getTexture("de.pvr"));
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	shap->drop();
	unit->drop();
	entity->drop();

	cubeModel->addAnimator(rotateAnimator);

	rotateAnimator->drop();


	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin();

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}