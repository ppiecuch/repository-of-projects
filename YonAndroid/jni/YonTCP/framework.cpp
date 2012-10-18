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

IModel* planeModel=NULL;
IModel* teapotModel=NULL;
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

#if 0
	//Client use UTF8 encode in sync mode
	ISocket* client=netMgr->createSocket(ENUM_SOCKET_PROTOCOL_TCP);
	client->init();
	client->open("192.168.23.100",8081);
	core::stringc str="helloÄãºÃ";
	str=i18nManager->convert(str.c_str(),ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	for(s32 i=0;i<20;++i)
		client->sendData(str.c_str(),str.length());
	Logger->debug("send str len:%d\r\n",str.length());
	client->close();
	client->drop();
#elif 0
	//Server use UTF8 encode in sync mode
	ISocket* server=netMgr->createSocket(ENUM_SOCKET_PROTOCOL_TCP);
	server->init();
	server->bindOn("192.168.23.100",8081);
	server->monitor();
	ISocket* client=server->acceptSocket();
	c8* buffer;
	s32 num=client->receiveData(buffer,20);
	core::stringc str;
	str.build(buffer,num);
	str=i18nManager->convert(str.c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030);
	Logger->debug("receive str:%s len:%d\r\n",str.c_str(),str.length());
	client->close();
	client->drop();
	server->close();
	server->drop();
#elif 0
	//Client use UTF8 encode in async mode
	ISocket* client=netMgr->createSocket(ENUM_SOCKET_PROTOCOL_TCP);
	client->init();
	//client->bindOn("192.168.23.100",8081);
	client->open("192.168.23.100",8081);
	client->setNonblocking(true);
	core::stringc str="helloÄãºÃ";
	str=i18nManager->convert(str.c_str(),ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	for(s32 i=0;i<20;++i)
		client->sendData(str.c_str(),str.length());
	Logger->debug("send str len:%d\r\n",str.length());
	client->close();
	client->drop();
#elif 1
	//Server use UTF8 encode in async mode
	ISocket* server=netMgr->createSocket(ENUM_SOCKET_PROTOCOL_TCP);
	server->init();
	server->bindOn("192.168.23.100",8081);
	server->setNonblocking(true);
	server->monitor();
	ISocket* client=server->acceptSocket();
	while(client==NULL)
	{
		core::yonSleep(200);
		Logger->debug("retry\n");
		client=server->acceptSocket();
		if(client)
		{
			c8* buffer;
			for(s32 i=0;i<21;++i)
			{
				s32 num=client->receiveData(buffer,11);
				if(num>0)
				{
					core::stringc str;
					str.build(buffer,num);
					str=i18nManager->convert(str.c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030);
					Logger->debug("receive str:%s len:%d\r\n",str.c_str(),str.length());
				}
				else
					Logger->debug("skip receive for block!\n");
			}
			client->close();
			client->drop();

			break;
		}
	}
	server->close();
	server->drop();
#endif
	
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
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}