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
ITimer* timer=NULL;
core::position2di ps[4];

ISceneNode* cubeModel=NULL;
ISceneNode* planeModel=NULL;
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
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	//IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createXYRectangle2D(-125,-125,125,125);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	SMaterial& material=planeModel->getMaterial(0);
	material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
	material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
	material.BlendDst=ENUM_BLEND_FACTOR_ONE;
	material.FrontFace=ENUM_FRONT_FACE_CW;
	material.setTexture(0,videoDriver->getTexture("aura.png"));
	planeModel->setPosition(core::vector3df(0,0,0));
	shap->drop();
	unit->drop();
	entity->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
float x=0;
void drawFrame(){

	u32 start=timer->getRealTime();
	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	//pCamera->setPosition(core::vector3df(x,x,300),true);

	//x+=0.1f;

	sceneMgr->render(videoDriver);
	//pCamera->render(videoDriver);

	gfAdapter->clearZ(1000);

#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	core::rectf r(0,1,1,0);
	for(u32 i=0;i<1;++i){
		//gfAdapter->drawRegion("shadow.png",r,randomizer->rand(0,400),randomizer->rand(0,400),128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER),true,0xFF0000FF);
		//gfAdapter->drawRegion("trans.png",r,randomizer->rand(0,400),randomizer->rand(0,400),128,64,ENUM_TRANS_NONE);
		
		TO_PS(250,120,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("shadow.png"),r,ps,ENUM_TRANS_NONE,true,0xFF0000FF);
		TO_PS(0,120,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_NONE);
		TO_PS(100,120,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_ROT180);
		TO_PS(200,120,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_MIRROR);
		TO_PS(300,120,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_MIRROR_ROT180);
		TO_PS(0,320,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("test.png"),r,ps,ENUM_TRANS_ROT90);
		TO_PS(50,170,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("shadow.png"),r,ps,ENUM_TRANS_NONE,true);
		TO_PS(100,30,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_MIRROR_ROT90);
		TO_PS(200,320,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_MIRROR_ROT270);
		TO_PS(300,320,128,64)
		gfAdapter->drawRegion(videoDriver->getTexture("trans.png"),r,ps,ENUM_TRANS_ROT270);
	}

		/*gfAdapter->drawVertexPrimitiveList(planeModel->getMaterial(0),
			planeModel->getEntity()->getUnit(0)->getShap()->getVertices(),
			planeModel->getEntity()->getUnit(0)->getShap()->getVertexCount(),
			planeModel->getEntity()->getUnit(0)->getShap()->getIndices(),
			planeModel->getEntity()->getUnit(0)->getShap()->getIndexCount(),
			planeModel->getEntity()->getUnit(0)->getShap()->getVertexType());*/
	

	gfAdapter->render(); 
	u32 end=timer->getRealTime();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,use:%d",videoDriver->getFPS(),end-start),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}