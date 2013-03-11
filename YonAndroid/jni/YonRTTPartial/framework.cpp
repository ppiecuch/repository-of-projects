#include "framework.h"

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ICamera* pCamera2=NULL;
ILogger* logger=NULL;

ISceneNode* cubeModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
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
			switch(evt.touchInput.type)
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
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera2=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0));
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	
	pCamera2->setEventReceivable(false);
	rtt = videoDriver->addRenderTargetTexture(core::dimension2d<u32>(1024,1024), "RTT",video::ENUM_COLOR_FORMAT_R8G8B8A8,true,false);
	pCamera2->onResize(rtt->getSize());

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
s32 temp=0;
void drawFrame(){

	videoDriver->begin();

	
	static core::recti area;
	static core::position2di ps[4];
	static core::rectf r(0,1,1,0);

	
	area.set(0,0,128,128);
	pCamera2->onResize(core::dimension2du(area.getWidth(),area.getHeight()));
	pCamera2->setDirty();
	pCamera2->render(videoDriver);
	videoDriver->setRenderTarget(rtt,true,true,COLOR_BLUE,true,area);


#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	gfAdapter->clearZ(1000);
	TO_PS(0,0,128,128)
	gfAdapter->drawRegion(videoDriver->getTexture("aura.png"),r,ps);
	gfAdapter->render(); 

	
	area.set(384,384,512,512);
	pCamera2->onResize(core::dimension2du(area.getWidth(),area.getHeight()));
	pCamera2->setDirty();
	pCamera2->render(videoDriver);
	videoDriver->setRenderTarget(rtt,true,true,COLOR_RED,true,area);

	
#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	gfAdapter->clearZ(1000);
	TO_PS(0,0,128,128);
	gfAdapter->drawRegion(videoDriver->getTexture("test-png24.png"),r,ps);
	gfAdapter->render(); 


	//»Ö¸´ÖÁÆÁÄ»»º³åÇø
	videoDriver->setRenderTarget(NULL,true,true,COLOR_DEFAULT);

	pCamera->setDirty();
	pCamera->render(videoDriver);
	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);

	gfAdapter->clearZ(1000);
	TO_PS(100,100,128,128);
	gfAdapter->drawRegion(rtt,r,ps);
	gfAdapter->render();

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}