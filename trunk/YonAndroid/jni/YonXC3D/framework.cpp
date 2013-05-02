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
//IRandomizer* randomizer=NULL;
ITimer* timer=NULL;
f32 factor=1.1f;

IAnimatedEntity* roleEntity=NULL;
IAnimatedEntity* weaponEntity;
IAnimatedEntity* rideEntity;
ISkinnedEntity* skeletonEntity=NULL;
IAnimatedSceneNode* roleNode;
scene::IBoneSceneNode* ridgeDummy;
scene::IBoneSceneNode* handDummy;
scene::IBoneSceneNode* weaponDummy;

//bool needLoad=false;

class CRoleAnimationEndCallback : public IAnimationEndCallBack{
public:
	virtual void onAnimationEnd(IAnimatedSceneNode* node){
		//Logger->setAppender(debug::MASK_APPENDER_VS);
		//YON_DEBUG("/********************************************/\r\n");
		//YON_DEBUG("/********************************************/\r\n");
		ISkinnedEntity* modelEntity=(ISkinnedEntity*)node->getEntity();
		modelEntity->useAnimationFrom(skeletonEntity);
		node->setFrameLoop(0,skeletonEntity->getFrameCount());

		node->setLoopMode(true);

		for(s32 i=0;i<node->getMaterialCount();++i)
		{
			node->getMaterial(i).Brightness=1;
		}
		//roleNode->setAnimationSpeed(0.3f);	
		//roleNode->setFrameLoop(1,1);
	}
};


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
				if(roleNode)
				{
					ISkinnedEntity* modelEntity=(ISkinnedEntity*)roleNode->getEntity();
					modelEntity->useAnimationFrom(modelEntity);
					roleNode->setFrameLoop(0,modelEntity->getFrameCount());

					roleNode->setLoopMode(false);

					for(s32 i=0;i<roleNode->getMaterialCount();++i)
					{
						roleNode->getMaterial(i).Brightness=2;
					}
				}
				else
				{
					const c8* roleName="knight_male_show.xc3d";
					roleEntity=sceneMgr->getEntity(roleName);
					roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
					roleNode->setName(roleName);
					roleEntity->drop();

					IAnimationEndCallBack* cb=new CRoleAnimationEndCallback();
					roleNode->setAnimationEndCallback(cb);
					cb->drop();

					roleNode->setLoopMode(false);
				}
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
				if(roleNode)
				{
					ISkinnedEntity* modelEntity=(ISkinnedEntity*)roleNode->getEntity();
					modelEntity->useAnimationFrom(modelEntity);
					roleNode->setFrameLoop(0,modelEntity->getFrameCount());

					roleNode->setLoopMode(false);

					for(s32 i=0;i<roleNode->getMaterialCount();++i)
					{
						roleNode->getMaterial(i).Brightness=2;
					}
				}
				else
				{
					//needLoad=true;
					const c8* roleName="knight_male_show.xc3d";
					roleEntity=sceneMgr->getEntity(roleName);
					roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
					roleNode->setName(roleName);
					roleEntity->drop();

					IAnimationEndCallBack* cb=new CRoleAnimationEndCallback();
					roleNode->setAnimationEndCallback(cb);
					cb->drop();

					roleNode->setLoopMode(false);
				}
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
	pCamera->setPosition(core::position3df(0,0,130),false);
	pCamera->setTarget(core::position3df(0,0,0));
	pCamera->setFar(5000);
	pCamera->setNear(1);
	logger=Logger;
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
	//fs->addWorkingDirectory("../media/xc3d",true);
	fs->addWorkingDirectory("../media/xc3d/new");
	fs->addWorkingDirectory("../media/ms3d",true);
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("media/png/");
	fs->addWorkingDirectory("media/xc3d",true);
	fs->addWorkingDirectory("media/ms3d",true);
#endif
#if 0
	const c8* roleName="knight_male_show.xc3d";

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();
#elif 0
	//测试动态加载
	const c8* skeletonName="knight_male_stand2.xc3d";
	skeletonEntity=(ISkinnedEntity*)sceneMgr->getEntity(skeletonName);
#elif 0
	//测试模型渲染速度
	const c8* roles[]={"knight_male_show.xc3d","knight_female_show.xc3d","ranger_male_show.xc3d","ranger_female_show.xc3d",
		"oracle_male_show.xc3d","oracle_female_show.xc3d","magician_male_show.xc3d","magician_female_show.xc3d"};

	for(s32 i=0;i<8;++i)
	{
		roleEntity=(ISkinnedEntity*)sceneMgr->getEntity(roles[i]);
		roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
		roleNode->setName(roles[i]);
		roleNode->setPosition(core::vector3df(-100+i*25,0,0));
		roleEntity->drop();
	}
#elif 1

	//Logger->setAppender(debug::MASK_APPENDER_NONE);

	//正常
	//const c8* roleName="knight_male_show.xc3d";
	//const c8* skeletonName="knight_male_stand2.xc3d";

	//正常
	//const c8* roleName="knight_female_show.xc3d";
	//const c8* skeletonName="knight_female_stand2.xc3d";

	//正常
	const c8* roleName="ranger_male_show.xc3d";
	const c8* skeletonName="ranger_male_stand2.xc3d";

	//正常
	//const c8* roleName="ranger_female_show.xc3d";
	//const c8* skeletonName="ranger_female_stand2.xc3d";

	//正常
	//const c8* roleName="oracle_male_show.xc3d";
	//const c8* skeletonName="oracle_male_stand2.xc3d";

	//披风跳动，切换跳帧(build_LocalAnimatedMatrix中没有使用useAnimationFrom->LocalMatrix)==》都正常了，但左脚在偏动(matrix2quaternion中直接>0判断有问题)==》正常
	//正常
	//const c8* roleName="oracle_female_show.xc3d";
	//const c8* skeletonName="oracle_female_stand2.xc3d";

	//正常
	//const c8* roleName="magician_male_show.xc3d";
	//const c8* skeletonName="magician_male_stand2.xc3d";

	//正常
	//const c8* roleName="magician_female_show.xc3d";
	//const c8* skeletonName="magician_female_stand2.xc3d";

	PROFILE_REGISTER_FRAME();

	PROFILE_START_CALL(PROFILE_ID_1,load);

	skeletonEntity=(ISkinnedEntity*)sceneMgr->getEntity(skeletonName);
	
	roleEntity=sceneMgr->getEntity(roleName);
	roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();

	IAnimationEndCallBack* cb=new CRoleAnimationEndCallback();
	roleNode->setAnimationEndCallback(cb);
	cb->drop();

	roleNode->setLoopMode(false);
	//roleNode->setFrameLoop(29,30);
	//roleNode->setAnimationSpeed(0.1f);	

	PROFILE_END_CALL(PROFILE_ID_1);

	PROFILE_REGISTER_FRAME();
#elif 0

	const c8* roleName="ghost.xc3d";
	const c8* dummyName="Bone23";
	const c8* weaponName="bow.xc3d";

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();

	weaponDummy=roleNode->getJointNode(dummyName);
	weaponDummy->setName(dummyName);

	weaponEntity=sceneMgr->getEntity(weaponName);
	scene::IAnimatedSceneNode* weapon =sceneMgr->addAnimatedSceneNode(weaponEntity,weaponDummy);
	weapon->setName(weaponName);
	weaponEntity->drop();


#elif 0
	const c8* roleName="knight_dummy.xc3d";
	const c8* dummyName="Dummy01";
	const c8* weaponName="bow.xc3d";

	roleEntity=sceneMgr->getEntity(roleName);
	IAnimatedSceneNode* roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
	roleNode->setName(roleName);
	roleEntity->drop();

	roleNode->setPosition(core::vector3df(0,0,200));

	weaponDummy=roleNode->getJointNode(dummyName);
	weaponDummy->setName(dummyName);

	weaponEntity=sceneMgr->getEntity(weaponName);
	scene::IAnimatedSceneNode* weapon =sceneMgr->addAnimatedSceneNode(weaponEntity,weaponDummy);
	weapon->setName(weaponName);
	weaponEntity->drop();

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

	/*if(needLoad&&roleNode==NULL)
	{
		const c8* roleName="knight_male_show.xc3d";
		roleEntity=sceneMgr->getEntity(roleName);
		roleNode=sceneMgr->addAnimatedSceneNode(roleEntity);
		roleNode->setName(roleName);
		roleEntity->drop();
	}*/

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	const core::vector3df& cameraPos=pCamera->getPosition();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,CAM:{%.2f,%.2f,%.2f}:frame:%.2f",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),cameraPos.x,cameraPos.y,cameraPos.z,roleNode?roleNode->getCurrentFrame():0),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED,5.f);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN,5.f);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE,5.f);

	videoDriver->end();
}
void destroy(){
	PROFILE_REPORT();
	if(skeletonEntity)
		skeletonEntity->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}