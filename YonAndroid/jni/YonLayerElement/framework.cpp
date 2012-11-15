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

f32 factor=1.1f;
ITexture* texture=NULL;
const static s32 LAYER_ID1=1;
const static s32 LAYER_ID2=2;
const static s32 ELEMENT1_COUNT=1000;
const static s32 ELEMENT2_COUNT=5000;
core::array<s32> ids;

#include "SDynamicShap.h"

core::array<core::stringc> images;
core::array<ITexture*> textures;

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
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	//fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/batch");
#elif defined(YON_COMPILE_WITH_ANDROID)
	//fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("media/batch");
#endif

	for(u32 i=1;i<=50;++i){
		images.push_back(core::stringc("%d.png",i));
		textures.push_back(videoDriver->getTexture(images[i-1]));
	}

	//texture=videoDriver->getTexture("de.png");

	gfAdapter->beginLayer(LAYER_ID1);
	for(s32 i=0;i<ELEMENT1_COUNT;++i)
	{
		s32 x=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().w);
		s32 y=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().h);
		texture=textures[randomizer->rand(0,textures.size()-1)];
		gfAdapter->beginElement(i);
		core::rectf r1(0,1,320.f/512.f,(256.f-243.f)/256.f);
		gfAdapter->drawRegion(texture,r1,x-57,y+19,320,243);
		core::rectf r2(319.f/512.f,1,332.f/512.f,(256.f-20.f)/256.f);
		gfAdapter->drawRegion(texture,r2,x,y,13,20);
		gfAdapter->endElement();

		if(i%2==0)
		ids.push_back(i);
	}
	ids.sort();
	gfAdapter->setDrawElements(ids);
	gfAdapter->endLayer();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
#define BATCH
void drawFrame(){

	videoDriver->begin(true,true,COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	gfAdapter->renderLayer(LAYER_ID1);
	
	static u32 start,end,diff;
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,DCL:%d,use:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall(),diff),core::position2di(0,10),COLOR_GREEN);

	videoDriver->end();
	end=timer->getRealTime();
	diff=end-start;
	start=timer->getRealTime();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}