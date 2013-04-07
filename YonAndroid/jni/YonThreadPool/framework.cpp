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

core::threadpool* pool=NULL;

class MyWork : public core::IWork{
public:
	static core::mutex s_mutex;
	//static core::semaphore s_mutex;
	static s32 counter;
	s32 count;
	MyWork():count(0){}
public:
	virtual void process(){
		s_mutex.wait();
		counter<<=1;
		counter-=1;
		count=counter;
		YON_DEBUG("process mywork:%d\r\n",count);
		s_mutex.notify();
		core::yonSleep(500);
	}
};
core::mutex MyWork::s_mutex;
//core::semaphore MyWork::s_mutex;
s32 MyWork::counter=1;

void addWork(){
	if(pool)
	{
		IWork* work=new MyWork();
		pool->push(work);
		work->drop();
	}
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
				//logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				//logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				s32 num=core::randomizer::rand(1,10);
				for(s32 i=0;i<num;++i)
					addWork();
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.xs[0],evt.touchInput.ys[0]);
				s32 num=core::randomizer::rand(1,10);
				for(s32 i=0;i<num;++i)
					addWork();
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
	
	YON_DEBUG("start createThreadPool()\r\n");
	pool=core::createThreadPool(5);
	pool->run();
	YON_DEBUG("end createThreadPool()\r\n");
	
	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,WORK:%u",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),pool->size()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	//可以多次调用stop
	//pool->stop();
	if(pool)
		//drop会自动调用stop
		pool->drop();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}