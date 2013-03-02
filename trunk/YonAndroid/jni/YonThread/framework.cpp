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
II18NManager* i18nManager=NULL;
INetManager* netMgr=NULL;

f32 factor=1.1f;

IThread* thread1=NULL;
IThread* thread2=NULL;
core::mutex mt;
s32 testNum=1;
struct SMyRunnable1 : public yon::IRunnable{
	virtual void run(){
		YON_DEBUG("start SMyRunnable1()\r\n");
		for(u32 i=0;i<10;++i)
		{
			//mt.wait();
			++testNum;
			YON_DEBUG("SMyRunnable1 num:%d\r\n",testNum);
			--testNum;
			YON_DEBUG("SMyRunnable1 num:%d\r\n",testNum);
			//mt.notify();
			core::yonSleep(50);
		}
		YON_DEBUG("end SMyRunnable1()\r\n");
	}
};

struct SMyRunnable2 : public yon::IRunnable{
	virtual void run(){
		YON_DEBUG("start SMyRunnable2()\r\n");
		for(u32 i=0;i<10;++i)
		{
			//mt.wait();
			testNum<<=1;
			YON_DEBUG("SMyRunnable2 num:%d\r\n",testNum);
			testNum>>=1;
			YON_DEBUG("SMyRunnable2 num:%d\r\n",testNum);
			//mt.notify();
			core::yonSleep(50);
		}
		YON_DEBUG("end SMyRunnable2()\r\n");
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
	i18nManager=engine->getI18NManager();
	netMgr=engine->getNetManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
#endif
	
	YON_DEBUG("start createThread()\r\n");
	SMyRunnable1* r1=new SMyRunnable1();;
	thread1=createThread(r1);
	r1->drop();
	SMyRunnable2* r2=new SMyRunnable2();;
	thread2=createThread(r2);
	r2->drop();
	thread1->start();
	thread2->start();
	YON_DEBUG("end thread->start()\r\n");
	
	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	if(thread1)
		thread1->drop();
	if(thread2)
		thread2->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}