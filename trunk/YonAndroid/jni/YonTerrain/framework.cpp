#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pOverlookCamera=NULL;
ICamera* pOrthoCamera=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
IRandomizer* randomizer=NULL;

IModel* planeModel=NULL;
IModel* teapotModel=NULL;
ITerrainModel* terrainModel=NULL;
video::ITexture* rtt=NULL;
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
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pOverlookCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_PERSP,NULL,core::vector3df(0,10000,0),core::vector3df(0,0,1),core::ORIGIN_VECTOR3DF,false);
	pOverlookCamera->setNear(1000);
	pOverlookCamera->setFar(30000);
	pOrthoCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	pCamera=sceneMgr->addCameraFPS();
	pCamera->setFar(1000);
	//pCamera->setNear(100);
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT,true);
	

	IShap *shap;
	IUnit* unit;
	IEntity* entity;
	IMaterial* material;

	/*shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	//teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();*/

	/*for(s32 i=-100;i<100;i+=30)
	{
		for(s32 j=-100;j<100;j+=30)
		{
			IModel* cube;
			shap=geometryFty->createCube(20,20,20);
			unit=geometryFty->createUnit(shap);
			entity=geometryFty->createEntity(unit);
			cube=sceneMgr->addModel(entity);
			cube->getMaterial(0)->setTexture(0,videoDriver->getTexture("test.png"));
			cube->setPosition(core::position3df(i,0,100+j));
			shap->drop();
			unit->drop();
			entity->drop();
		}
	}*/

	rtt = videoDriver->addRenderTargetTexture(core::dimension2d<u32>(128,128), "RTT");

	shap=geometryFty->createXYRectangle2D(272,112,400,240);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	planeModel->setPosition(core::vector3df(0,0,0));
	material->setTexture(0,rtt);
	shap->drop();
	unit->drop();
	entity->drop();
	

	terrainModel=sceneMgr->addTerrainModel(NULL,ORIGIN_VECTOR3DF,ORIGIN_VECTOR3DF,core::vector3df(10,1,10));
	IImage* image=videoDriver->createImageFromFile("heightmap512.png",true);
	terrainModel->loadHeightMap(image,ENUM_PATCH_SIZE_17);
	//terrainModel->loadHeightMap(image);
	image->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	rtt->beginRTT(true,true,video::SColor(0xFF133E67));
	sceneMgr->setActiveCamera(pOverlookCamera);
	sceneMgr->render(videoDriver);
	pCamera->getViewFrustum()->render(videoDriver);
	const core::aabbox3df& aabb=pCamera->getViewFrustum()->getBoundingBox();
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->setMaterial(DEFAULT_MATERIAL);
	vector3df corners[8];
	aabb.getCorners(corners);
	videoDriver->draw3DLine(corners[0],corners[1],COLOR_GREEN);
	videoDriver->draw3DLine(corners[0],corners[2],COLOR_GREEN);
	videoDriver->draw3DLine(corners[0],corners[4],COLOR_GREEN);
	videoDriver->draw3DLine(corners[1],corners[3],COLOR_GREEN);
	videoDriver->draw3DLine(corners[1],corners[5],COLOR_GREEN);
	videoDriver->draw3DLine(corners[2],corners[3],COLOR_GREEN);
	videoDriver->draw3DLine(corners[2],corners[6],COLOR_GREEN);
	videoDriver->draw3DLine(corners[3],corners[7],COLOR_GREEN);
	videoDriver->draw3DLine(corners[4],corners[5],COLOR_GREEN);
	videoDriver->draw3DLine(corners[4],corners[6],COLOR_GREEN);
	videoDriver->draw3DLine(corners[5],corners[7],COLOR_GREEN);
	videoDriver->draw3DLine(corners[6],corners[7],COLOR_GREEN);
	rtt->endRTT(true);

	sceneMgr->setActiveCamera(pCamera);
	planeModel->setVisible(false);

	//const core::vector3df trot=teapotModel->getRotation();
	//teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	sceneMgr->render(videoDriver);

	core::vector3df pos = pCamera->getAbsolutePosition();
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,cam(%.2f,%.2f,%.2f)",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),pos.x,pos.y,pos.z),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	sceneMgr->setActiveCamera(pOrthoCamera);
	planeModel->setVisible(true);
	terrainModel->setVisible(false);
	sceneMgr->render(videoDriver);
	videoDriver->end();

	terrainModel->setVisible(true);
	sceneMgr->setActiveCamera(pCamera);
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}