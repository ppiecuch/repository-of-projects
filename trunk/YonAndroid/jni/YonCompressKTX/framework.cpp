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
f32 factor=1.1f;
core::position2di ps[4];

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

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	if(engine->available()==false)
		return false;
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/compressTexture",true);
	//fs->addWorkingDirectory("../media");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/compressTexture",true);
#endif

	/*IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createCube(150,150,150);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	SMaterial& material=cubeModel->getMaterial(0);
	material.setTexture(0,videoDriver->getTexture("de.pkm"));
	//material.MaterialType=ENUM_MATERIAL_TYPE_TRANSPARENT;
	cubeModel->setPosition(core::vector3df(100,100,0));
	shap->drop();
	unit->drop();
	entity->drop();

	SAnimatorParam rotateAnimatorParam;
	rotateAnimatorParam.type=ENUM_ANIMATOR_TYPE_ROTATE;
	rotateAnimatorParam.animatorRotate.rotateSpeed=0.1f;
	IAnimator* rotateAnimator=animatorFty->createAnimator(rotateAnimatorParam);
	cubeModel->addAnimator(rotateAnimator);
	rotateAnimator->drop();*/

#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	TO_PS(0,0,512,256)


	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin();

	sceneMgr->render(videoDriver);

	static core::rectf r(0,1,1,0);
	gfAdapter->clearZ(1000);
	ITexture* texture=videoDriver->getTexture("de.ktx");
	//gfAdapter->drawFill(texture,r,ps,ENUM_TRANS_NONE,0xFF00FF00);
	gfAdapter->drawRegion(texture,r,ps,ENUM_TRANS_NONE,true);
	gfAdapter->render();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}