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

IAnimatedEntity* roleEntity;
IAnimatedEntity* weaponEntity;
IAnimatedEntity* rideEntity;
scene::IBoneSceneNode* ridgeDummy;
scene::IBoneSceneNode* handDummy;

bool init(void *pJNIEnv,ICallback* pcb,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.appPath=appPath;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCameraFPS();
	pCamera->setFar(5000);
	pCamera->setNear(1);
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
	fs->addWorkingDirectory("../media/xc3d",true);
	fs->addWorkingDirectory("../media/ms3d",true);
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
	fs->addWorkingDirectory("media/xc3d",true);
	fs->addWorkingDirectory("media/ms3d",true);
#endif

#if 0
	const c8* roleName="ghost.xc3d";
	//const c8* dummyName="Bone01";
	//const c8* weaponName="bow.xc3d";

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();
#else
	const c8* roleName="archer.xc3d";
	const c8* ridgeDummyName="hp_horse0_attach";
	const c8* handDummyName="hp_hero1_attach";
	const c8* weaponName="bow.xc3d";
	const c8* rideName="horse.xc3d";

	rideEntity=sceneMgr->getEntity(rideName);
	IAnimatedSceneNode* rideNode=sceneMgr->addAnimatedSceneNode(rideEntity);
	rideNode->setName(rideName);
	rideEntity->drop();

	rideNode->setPosition(core::vector3df(0,0,200));
	rideNode->setRotation(core::vector3df(0,90,0));

	ridgeDummy=rideNode->getJointNode(ridgeDummyName);
	ridgeDummy->setName(ridgeDummyName);

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity,ridgeDummy);
	roleNode->setName(roleName);
	roleEntity->drop();

	roleNode->setRotation(core::vector3df(90,0,0));

	handDummy=roleNode->getJointNode(handDummyName);
	handDummy->setName(handDummyName);
	//YON_DEBUG("handDummy:%s,%08X\r\n",handDummy->getName(),handDummy);
	weaponEntity=sceneMgr->getEntity(weaponName);

	scene::IAnimatedSceneNode* weapon =sceneMgr->addAnimatedSceneNode(weaponEntity,handDummy);
	//weapon->setRotation(core::vector3df(-90,0,0));
	weapon->setName(weaponName);
	//weapon->getMaterial(0).PolygonMode=ENUM_POLYGON_MODE_LINE;
	YON_DEBUG("handDummy:%s,weapon:%s,parent:%s\r\n",handDummy->getName(),weapon->getName(),weapon->getParent()->getName());
	weaponEntity->drop();
#endif
	//roleNode->setFrameLoop(12,13);
	//roleNode->setAnimationSpeed(0.3f);
	/*
	IAnimatedEntity* entity=sceneMgr->getEntity("knight.xc3d");
	//entity->getUnit(0)->getMaterial().PolygonMode=ENUM_POLYGON_MODE_LINE;
	IAnimatedSceneNode* node=sceneMgr->addAnimatedSceneNode(entity);
	//node->setScale(core::vector3df(0.2f,0.2f,0.2f));
	//node->setFrameLoop(10,10);
	//node->setAnimationSpeed(0.3f);
	//临时需要drop，添加EntityCache后就不用了
	entity->drop();
	*/

	//在世界坐标系下，以下结果表现为先沿Y轴偏移1个单位，再绕X转旋转90度，结果为（0，0，1）
	//列向量右乘矩阵
	/*core::matrix4f m1(true);
	m1.setRotation(90,1,0,0);

	core::matrix4f m2(true);
	m2.setTranslation(0,1,0);

	core::matrix4f mr=m1*m2;
	mr.print();

	core::vector3df v=core::ORIGIN_VECTOR3DF;
	mr.transformVect(v);
	v.print();*/



	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	const core::vector3df& cameraPos=pCamera->getPosition();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,CAM:{%.2f,%.2f,%.2f}",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),cameraPos.x,cameraPos.y,cameraPos.z),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}