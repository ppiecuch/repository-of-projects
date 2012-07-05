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
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

	/*
	for(u32 i=0;i<100;++i){
		Logger->debug("rand:%d\n",randomizer->rand());
		Logger->debug("rand:%d\n",randomizer->rand(0,20));
	}
	*/
	

	IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

	/*ISound* sound=audioDriver->getSound("bg.ogg");
	sound->setLooping(true);
	//sound->setGain(0.5f);
	sound->play();
	//sound=audioDriver->getSound("helloworld.wav");
	//sound->play();*/

	shap=geometryFty->createCube(64,64,64);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	//material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	//material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
	cubeModel->setPosition(core::vector3df(100,100,0));
	//material->setTexture(0,videoDriver->getTexture("png8/120.png"));
	//material->setTexture(0,videoDriver->getTexture("error.png"));
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

	shap=geometryFty->createXYRectangle2D(-125,-125,125,125);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	//material->setMaterialType(ENUM_MATERIAL_TYPE_BLEND);
	//material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
	//material->setWrapModeU(0,ENUM_WRAP_MODE_CLAMP_TO_EDGE);
	//material->setWrapModeV(0,ENUM_WRAP_MODE_CLAMP_TO_EDGE);
	//material->setBlendSrcFactor(ENUM_BLEND_FACTOR_SRC_ALPHA);
	//material->setBlendDstFactor(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
	planeModel->setPosition(core::vector3df(0,0,0));
	//material->setTexture(0,videoDriver->getTexture("5.png"));
	material->setTexture(0,videoDriver->getTexture("hierogram1.png"));
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

	const core::vector3df crot=cubeModel->getRotation();
	cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	const core::vector3df trot=teapotModel->getRotation();
	teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	/*const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.9f;
	else if(psca.x<2)
		factor=1.1f;
	planeModel->setScale(psca*factor);*/

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	//videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);

	/*gfAdapter->beginBatch(0);
	gfAdapter->drawImage("nav.png",0,0,128,128,0,0,true);
	//gfAdapter->drawImage("nav.png",0,0,128,128,100,0,true);
	
	//gfAdapter->drawImage("test.png",100,100,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER));
	//gfAdapter->drawImage("firework.png",100,100);
	//gfAdapter->drawImage("test.png",100,100,(MASK_ACTHOR)(MASK_ACTHOR_RIGHT|MASK_ACTHOR_BOTTOM));

	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_NONE,0,120);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_ROT180,100,120);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_MIRROR,200,120);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_MIRROR_ROT180,300,120);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_ROT90,0,320);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_MIRROR_ROT90,100,320);
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_MIRROR_ROT270,200,320,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER));
	gfAdapter->drawRegion("trans.png",0,0,128,64,ENUM_TRANS_ROT270,300,320,(MASK_ACTHOR)(MASK_ACTHOR_RIGHT|MASK_ACTHOR_BOTTOM));
	gfAdapter->endBatch();*/

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}