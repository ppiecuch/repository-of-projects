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
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
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

	/*
	for(u32 i=0;i<100;++i){
	Logger->debug("rand:%d\n",randomizer->rand());
	Logger->debug("rand:%d\n",randomizer->rand(0,20));
	}
	*/


	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

#if 0

	//TODO BUG:Mask模型显示不正常
	//如果关闭teapot或aura则可正常显示
	//跟踪发现：
	//只要把
	//if(current.EnvSetting.EnvMode!=last.EnvSetting.EnvMode)
	//{
	//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, current.EnvSetting.EnvMode);
	//}
	//的if部分去掉就OK了
	//待分析

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	//{
	//	SMaterial& material=teapotModel->getMaterial(0);
	//	material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
	//	material.setTexture(0,videoDriver->getTexture("aura.png"));
	//}
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
	entity->drop();

	//shap=geometryFty->createXYRectangle2D2T(-75,-150,75,150,0,0,1,0.1f);
	shap=geometryFty->createXYRectangle2D2T(-256,-128,256,128,0,0,1,0.1f);
	unit=geometryFty->createUnit(shap);
	//unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
	entity=geometryFty->createEntity(unit);
	IModel* waterfallModel=sceneMgr->addModel(entity);
	{
		SMaterial& material=waterfallModel->getMaterial(0);
		material.MaterialType=ENUM_MATERIAL_TYPE_MASK;
		material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
		material.BlendDst=ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		material.setTexture(0,videoDriver->getTexture("fire.png"));
		material.setTexture(1,videoDriver->getTexture("xyj.png"));
	}
	waterfallModel->setPosition(core::vector3df(50,100,120));
	shap->drop();
	unit->drop();
	entity->drop();
	SAnimatorParam aniParam;
	aniParam.type=ENUM_ANIMATOR_TYPE_UV;
	aniParam.animatorUV.unitIndex=0;
	aniParam.animatorUV.stage=0;
	aniParam.animatorUV.translate.u=0;
	aniParam.animatorUV.translate.w=0;
	aniParam.animatorUV.translate.v=-0.05f;
	IAnimator* uvAnimator=animatorFty->createAnimator(aniParam);
	waterfallModel->addAnimator(uvAnimator);
	uvAnimator->drop();

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

	//const core::vector3df crot=cubeModel->getRotation();
	//cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	/*const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

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