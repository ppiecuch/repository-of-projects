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
//IRandomizer* randomizer=NULL;

ISceneNode* cubeModel=NULL;
ISceneNode* weedModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
video::ITexture* rtt=NULL;
f32 factor=1.1f;

ISound* sound=NULL;

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

bool init(void *pJNIEnv,ICallback* pcb,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=60;
	params.appPath=appPath;
	params.resourcesPath=resPath;
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
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../Yon/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

#if 1

	videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE, true);

	//IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addSceneNode(entity);
	//cubeModel->debugName="cubeModel";
	{
		SMaterial& material=cubeModel->getMaterial(0);
		rtt = videoDriver->addRenderTargetTexture(core::dimension2d<u32>(256,256), "RTT",video::ENUM_COLOR_FORMAT_R8G8B8A8);
		material.setTexture(0,rtt);
		//material->setTexture(0,videoDriver->getTexture("aura_.png"));
		material.MaterialType=ENUM_MATERIAL_TYPE_TRANSPARENT_REF;
	}
	cubeModel->setPosition(core::vector3df(100,100,0));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createTeapot(7,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addSceneNode(entity);
	teapotModel->setPosition(core::vector3df(0,-70,0));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	//shap->setVertexHardwareBufferUsageType(ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	//shap->setIndexHardwareBufferUsageType(ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addSceneNode(entity);
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

	


	sound=audioDriver->getSound("helloworld.wav");
	sound->play();
	sound=audioDriver->getSound("bg.ogg");
	sound->setLooping(true);
	sound->setGain(0.5f);
	sound->play();
	

#endif
	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	//Logger->debug("beginDriver\r\n");
	videoDriver->begin(true,true,COLOR_DEFAULT);
#if 1
	//rtt->beginRTT(true,true,video::SColor(0xFF133E67));
	//Logger->debug("beginRTT\r\n");
	//rtt->beginRTT(true,true,video::SColor(0x00000000));
	videoDriver->setRenderTarget(rtt);

	teapotModel->setVisible(true);
	planeModel->setVisible(false);
	cubeModel->setVisible(false);

	sceneMgr->render(videoDriver);

	teapotModel->setVisible(false);
	cubeModel->setVisible(true);
	planeModel->setVisible(true);


	//rtt->endRTT(true);
	//Logger->debug("endRTT\r\n");
	videoDriver->setRenderTarget(NULL,true,true,COLOR_DEFAULT);

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

	//videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(video::ENUM_TRANSFORM_WORLD,IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);

	//Logger->drawString(videoDriver,core::stringc("FPS:%d,%.2f",videoDriver->getFPS(),sound->getSecondOffset()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

#endif 

	//*(int*)0 = 1;

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	videoDriver->end();
	//Logger->debug("endDriver\r\n");
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}