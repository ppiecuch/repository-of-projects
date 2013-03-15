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
II18NManager* i18nManager=NULL;
INetManager* netMgr=NULL;

f32 factor=1.1f;

IThread* thread1=NULL;
IThread* thread2=NULL;
core::mutex mt;
s32 testNum=1;
struct SMyRunnable1 : public yon::IRunnable{
	virtual void run(){
		YON_INFO("start SMyRunnable1()\r\n");
		for(u32 i=0;i<100;++i)
		{
			mt.wait();
			++testNum;
			YON_DEBUG_BREAK_IF(testNum!=2);
			core::yonSleep(50);
			--testNum;
			YON_DEBUG_BREAK_IF(testNum!=1);
			mt.notify();
			core::yonSleep(50);
			YON_DEBUG("SMyRunnable1(%d)\r\n",i);
			thread1->suspend();
		}
		core::yonSleep(50);
		YON_INFO("end SMyRunnable1():%d\r\n",testNum);
	}
};

struct SMyRunnable2 : public yon::IRunnable{
	virtual void run(){
		YON_INFO("start SMyRunnable2()\r\n");
		for(u32 i=0;i<100;++i)
		{
			mt.wait();
			testNum<<=2;
			YON_DEBUG_BREAK_IF(testNum!=4);
			core::yonSleep(50);
			testNum>>=2;
			YON_DEBUG_BREAK_IF(testNum!=1);
			mt.notify();
			core::yonSleep(50);
			YON_DEBUG("SMyRunnable2(%d)\r\n",i);
		}
		core::yonSleep(50);
		YON_INFO("end SMyRunnable2():%d\r\n",testNum);
	}
};


IThread* thread3=NULL;
IThread* thread4=NULL;
core::mutex ms;
s32 testCount=1;
struct SMyRunnable3 : public yon::IRunnable{
	virtual void run(){
		YON_INFO("start SMyRunnable3()\r\n");
		for(u32 i=0;i<100;++i)
		{
			ms.wait();
			++testCount;
			YON_DEBUG_BREAK_IF(testCount!=2);
			core::yonSleep(50);
			--testCount;
			YON_DEBUG_BREAK_IF(testCount!=1);
			ms.notify();
			core::yonSleep(50);
			YON_DEBUG("SMyRunnable3(%d)\r\n",i);
		}
		core::yonSleep(50);
		YON_INFO("end SMyRunnable3():%d\r\n",testCount);
	}
};

struct SMyRunnable4 : public yon::IRunnable{
	virtual void run(){
		YON_INFO("start SMyRunnable4()\r\n");
		for(u32 i=0;i<100;++i)
		{
			ms.wait();
			testCount<<=2;
			YON_DEBUG_BREAK_IF(testCount!=4);
			core::yonSleep(50);
			testCount>>=2;
			YON_DEBUG_BREAK_IF(testCount!=1);
			ms.notify();
			core::yonSleep(50);
			YON_DEBUG("SMyRunnable4(%d)\r\n",i);
		}
		core::yonSleep(50);
		YON_INFO("end SMyRunnable4():%d\r\n",testCount);
	}
};

void swap()
{
	if(thread1->getState()==ENUM_THREAD_STATE_RUNNING)
		thread1->suspend();
	else 
		thread1->start();

	/*
	if(thread2->getState()==ENUM_THREAD_STATE_RUNNING)
		thread2->suspend();
	else 
		thread2->start();

	if(thread3->getState()==ENUM_THREAD_STATE_RUNNING)
		thread3->suspend();
	else 
		thread3->start();

	if(thread4->getState()==ENUM_THREAD_STATE_RUNNING)
		thread4->suspend();
	else 
		thread4->start();
	*/
}

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
				swap();
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				logger->debug("[R]%.2f,%.2f\n",evt.touchInput.xs[0],evt.touchInput.ys[0]);
				swap();
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.resourcesPath=resPath;
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
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/png/");
#endif
	
	YON_DEBUG("start createThread()\r\n");
	SMyRunnable1* r1=new SMyRunnable1();;
	thread1=createThread(r1);
	r1->drop();
	thread1->start();

	/*
	SMyRunnable2* r2=new SMyRunnable2();;
	thread2=createThread(r2);
	r2->drop();
	//thread2->start();

	SMyRunnable3* r3=new SMyRunnable3();;
	thread3=createThread(r3);
	r3->drop();
	SMyRunnable4* r4=new SMyRunnable4();;
	thread4=createThread(r4);
	r4->drop();
	//thread3->start();
	//thread4->start();
	*/
	YON_DEBUG("end thread->start()\r\n");
	
	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	if(thread1)
		thread1->drop();
	if(thread2)
		thread2->drop();

	if(thread3)
		thread3->drop();
	if(thread4)
		thread4->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}