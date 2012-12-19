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
ITimer* timer=NULL;
f32 factor=1.1f;

IAnimatedEntity* roleEntity=NULL;
IAnimatedEntity* weaponEntity;
IAnimatedEntity* rideEntity;
ISkinnedEntity* skeletonEntity;
IAnimatedSceneNode* roleNode;
scene::IBoneSceneNode* ridgeDummy;
scene::IBoneSceneNode* handDummy;
scene::IBoneSceneNode* weaponDummy;

bool needLoad=false;

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



class CRoleAnimationEndCallback : public IAnimationEndCallBack{
public:
	virtual void onAnimationEnd(IAnimatedSceneNode* node){
		Logger->setAppender(debug::MASK_APPENDER_VS);
		YON_DEBUG("/********************************************/\r\n");
		YON_DEBUG("/********************************************/\r\n");

		ISkinnedEntity* modelEntity=(ISkinnedEntity*)node->getEntity();
		modelEntity->useAnimationFrom(skeletonEntity);
		node->setFrameLoop(0,skeletonEntity->getFrameCount());

		node->setLoopMode(true);
		//roleNode->setAnimationSpeed(0.3f);	
		//roleNode->setFrameLoop(0,0);
	}
};

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
	timer=engine->getTimer();
	pCamera=sceneMgr->addCameraFPS();
	pCamera->setFar(5000);
	pCamera->setNear(1);
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
	fs->addWorkingDirectory("../media/xc3d",true);
	fs->addWorkingDirectory("../media/ms3d",true);
	fs->addWorkingDirectory("../media/dae",true);
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("media/png/");
	fs->addWorkingDirectory("media/xc3d",true);
	fs->addWorkingDirectory("media/ms3d",true);
#endif

#if 1
	const c8* roleName="oracle_female_stand2.dae";

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();

	roleNode->setFrameLoop(29,30);
#elif 1
	Logger->setAppender(debug::MASK_APPENDER_NONE);

	const c8* roleName="knight_male_show.dae";
	const c8* skeletonName="knight_male_stand2.dae";

	skeletonEntity=(ISkinnedEntity*)sceneMgr->getEntity(skeletonName);

	roleEntity=sceneMgr->getEntity(roleName);
	roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();

	IAnimationEndCallBack* cb=new CRoleAnimationEndCallback();
	roleNode->setAnimationEndCallback(cb);
	cb->drop();

	roleNode->setLoopMode(false);
#endif

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	const core::vector3df& cameraPos=pCamera->getPosition();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,CAM:{%.2f,%.2f,%.2f}:frame:%.2f",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),cameraPos.x,cameraPos.y,cameraPos.z,roleNode?roleNode->getCurrentFrame():0),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	videoDriver->end();
}
void destroy(){
	if(skeletonEntity)
		skeletonEntity->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}