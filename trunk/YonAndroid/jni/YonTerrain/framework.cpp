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

IModel* teapotModel=NULL;
ITerrainModel* terrainModel=NULL;
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
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	//pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_PERSP,NULL,core::vector3df(0,50,300));
	pCamera=sceneMgr->addCameraFPS();
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

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	//teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();

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

	terrainModel=sceneMgr->addTerrainModel(NULL,ORIGIN_VECTOR3DF,ORIGIN_VECTOR3DF,core::vector3df(10,1,10));
	IImage* image=videoDriver->createImageFromFile("heightmap16.png",true);
	terrainModel->loadHeightMap(image,ENUM_PATCH_SIZE_5);
	//terrainModel->loadHeightMap(image);
	image->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	//const core::vector3df trot=teapotModel->getRotation();
	//teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	sceneMgr->render(videoDriver);

	core::vector3df pos = pCamera->getAbsolutePosition();
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,cam(%.2f,%.2f,%.2f)",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),pos.x,pos.y,pos.z),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}