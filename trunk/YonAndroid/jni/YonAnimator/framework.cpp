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
	fs->addWorkingDirectory("media/");
#endif

	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createSphere(40);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	IModel* sphereModel=sceneMgr->addModel(entity);
	sphereModel->setPosition(core::vector3df(40,0,0));
	material=sphereModel->getMaterial(0);
	//material->setPolygonMode(ENUM_POLYGON_MODE_LINE);
	material->setTexture(0,videoDriver->getTexture("earth.png"));
	shap->drop();
	unit->drop();
	entity->drop();
	SAnimatorParam rotateAnimatorParam;
	rotateAnimatorParam.type=ENUM_ANIMATOR_TYPE_ROTATE;
	rotateAnimatorParam.animatorRotate.rotateSpeed=0.1f;
	IAnimator* rotateAnimator=animatorFty->createAnimator(rotateAnimatorParam);
	sphereModel->addAnimator(rotateAnimator);
	rotateAnimator->drop();

	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	IModel* cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	//cubeModel->setPosition(core::vector3df(50,100,120));
	material->setMaterialType(ENUM_MATERIAL_TYPE_BLEND);
	//material->setAlphaSource(ENUM_ALPHA_SOURCE_VERTEX);
	material->setTexture(0,videoDriver->getTexture("test-png24.png"));
	shap->drop();
	unit->drop();
	entity->drop();
	SAnimatorParam alphaAnimatorParam;
	alphaAnimatorParam.type=ENUM_ANIMATOR_TYPE_ALPHA;
	alphaAnimatorParam.animatorAlpha.unitIndex=0;
	alphaAnimatorParam.animatorAlpha.minValue=0;
	alphaAnimatorParam.animatorAlpha.maxValue=255;
	alphaAnimatorParam.animatorAlpha.increment=1;
	IAnimator* alphaAnimator=animatorFty->createAnimator(alphaAnimatorParam);
	cubeModel->addAnimator(alphaAnimator);
	alphaAnimator->drop();


	shap=geometryFty->createXYRectangle2D2T(-75,-150,75,150,0,0,1,0.1f);
	unit=geometryFty->createUnit(shap);
	//unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
	entity=geometryFty->createEntity(unit);
	IModel* waterfallModel=sceneMgr->addModel(entity);
	material=waterfallModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_MASK);
	waterfallModel->setPosition(core::vector3df(50,100,120));
	material->setTexture(0,videoDriver->getTexture("waterfall.png"));
	material->setTexture(1,videoDriver->getTexture("maskalpha.png"));
	shap->drop();
	unit->drop();
	entity->drop();
	SAnimatorParam aniParam;
	aniParam.type=ENUM_ANIMATOR_TYPE_UV;
	aniParam.animatorUV.unitIndex=0;
	aniParam.animatorUV.stage=0;
	aniParam.animatorUV.translate.u=0;
	aniParam.animatorUV.translate.w=0;
	aniParam.animatorUV.translate.v=0.02f;
	IAnimator* uvAnimator=animatorFty->createAnimator(aniParam);
	waterfallModel->addAnimator(uvAnimator);
	uvAnimator->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

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