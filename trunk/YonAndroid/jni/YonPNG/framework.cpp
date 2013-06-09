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

ISceneNode* planeLPNG8=NULL;
ISceneNode* planeLPNG24=NULL;
ISceneNode* planeLAPNG8=NULL;
ISceneNode* planeLAPNG24=NULL;
ISceneNode* planeRGBPNG8=NULL;
ISceneNode* planeRGBPNG24=NULL;
ISceneNode* planeRGBAPNG8=NULL;
ISceneNode* planeRGBAPNG24=NULL;
ISceneNode* teapotModel=NULL;
f32 factor=1.1f;

core::position2di ps[4];

#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

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

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
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
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/png/");
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
	fs->addWorkingDirectory("media/");
#endif

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,true);
	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,true);
	

	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	/*shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
	teapotModel->setPosition(core::vector3df(50,-50,0));
	shap->drop();
	unit->drop();
	entity->drop();*/

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	//const core::vector3df trot=teapotModel->getRotation();
	//teapotModel->setRotation(core::vector3df(trot.x+0.2f,trot.y-3.5f ,trot.z-0.5f));

	sceneMgr->render(videoDriver);

	static core::rectf r(0,1,1,0);
	static core::rectf r1(0.25f,0.75f,0.75f,0.25f);

	gfAdapter->clearZ(1000);
	//gfAdapter->drawImage("lpng8.png",0,0,32,32,10,10);
	//gfAdapter->drawImage("lpng24.png",0,0,32,32,50,10);
	//gfAdapter->drawImage("bigfont.png",0,0,64,32,100,10,true);
	//gfAdapter->drawImage("lapng24.png",0,0,64,32,170,10,true);

	//gfAdapter->drawImage("rgbpng8.png",0,0,64,64,10,100);
	//gfAdapter->drawImage("rgbpng24.png",0,0,64,64,100,100);

	//gfAdapter->drawImage("rgbapng8.png",0,0,256,256,10,200,true);
	//gfAdapter->drawImage("rgbapng24.png",0,0,256,256,50,50,true);

	//gfAdapter->drawImage("fixerror.png",0,0,32,32,340,10,true);

	//gfAdapter->drawImage("interlace.png",0,0,128,128,0,200,true);

	//gfAdapter->drawImage("cd.png",0,0,64,64,64,64,true);
	ITexture* texture1=videoDriver->getTexture("rgbapng24.png");
	ITexture* texture=videoDriver->getTexture("cd.png");
	
	TO_PS(0,0,256,256);
	gfAdapter->drawRegion(texture1,r,ps,ENUM_TRANS_NONE,true);

	TO_PS(50,50,64,64);
	gfAdapter->drawRegion(texture,r,ps,ENUM_TRANS_NONE,true);

	TO_PS(128,128,128,128);
	gfAdapter->drawRegion(texture1,r1,ps,ENUM_TRANS_NONE,true);

	gfAdapter->render();

	Logger->drawString(videoDriver,core::stringc("FPS:%d,DCL:%d",videoDriver->getFPS(),videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}