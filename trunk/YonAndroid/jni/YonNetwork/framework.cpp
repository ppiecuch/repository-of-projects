#include "cmdpacket.h"
#include "gamenetwork.h"
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
II18NManager* i18nManager=NULL;
ITimer* timer=NULL;

// 常量定义
const int kServerNameMaxLength = 32;
const int serverDscr           = 64; 
// 服务器信息
// 0维护（维护状态）、1正常（500人以下）、2繁忙（单服500人以上）、3爆满（达到单服人数上限） 
struct SGameServerInfo
{
	int         	id;                                      //服务器ID
	char			name[kServerNameMaxLength];              //服务器名称
	char			ip[kServerNameMaxLength];                //服务器ip
	short			port;                                    //服务器端口
	unsigned char	status;                                  //服务器状态标识
	unsigned char   zoneId;                                  //服务器分区标识
	unsigned char   rolec;                                   //角色数量
	char            serDscr[serverDscr];                     //服务器名称
	bool            newSev;                                 //是否新服
	bool            recommond;                               //是否推荐                              
};

char		mServerKey[64];

class DlgtTest{
public:
	void test(){
		YON_DEBUG("disconnect\r\n");
	}
	void scCmdLoginSuccess(CCmdPacket* msg){
		YON_DEBUG("scCmdLoginSuccess\r\n");

		//char		mServerKey[64];
		msg->ReadString(mServerKey);
		unsigned char serverListLen;
		msg->ReadByte(&serverListLen);
		SGameServerInfo	serverInfo;
		core::stringc testStr;
		YON_DEBUG("CGameState_Login::CmdLoginSuccess - ServerList: \n");
		for (unsigned char i = 0; i < serverListLen; i++)
		{
			msg->ReadInt32(&serverInfo.id);
			msg->ReadString(serverInfo.name);		
			msg->ReadString(serverInfo.ip);
			msg->ReadShort(&serverInfo.port);
			msg->ReadByte(&serverInfo.status);
			msg->ReadByte(&serverInfo.zoneId);
			msg->ReadByte(&serverInfo.rolec);
			msg->ReadString(serverInfo.serDscr);
			msg->ReadByte((unsigned char *)&serverInfo.newSev);
			msg->ReadByte((unsigned char *)&serverInfo.recommond);

			YON_DEBUG("(id,name,ip,port,status) - (%d, %s, %s, %d, %d) \r\n", serverInfo.id, serverInfo.name, serverInfo.ip, serverInfo.port, serverInfo.status);
		}
		YON_DEBUG("CGameState_Login::CmdLoginSuccess - ServerList End \n");

		GameNetwork()->Close();
		GameNetwork()->Init();
		GameNetwork()->Connect("221.130.10.112",30001);
		GetPacketMgr()->sendC2SPlayerListReqPacket("5F19E5E739FC27253468142BFE920D47");
	}
};
DlgtTest test;


class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt)
	{
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
				/*event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);*/
				for(u32 i=0;i<1;++i)
				{
					SPacketBuffer* buffer=GameNetwork()->getBuffer();
					buffer->Buffer=i18nManager->convert("hello你好");
					GameNetwork()->SendPacket(buffer);
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
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
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
	params.pCallback=pcb;
	//params.fpsLimit=10;
	params.appPath=appPath;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	i18nManager=engine->getI18NManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	//fs->addWorkingDirectory("../media/",true);
	fs->addWorkingDirectory("../Yon/");
	//fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/",true);
#endif

	CCmdPacket::setFileSystem(fs);

	IDelegateVoid* dlgt=createDelegateVoid<DlgtTest>(&test,&DlgtTest::test);
	GameNetwork()->setOnDisconnectDelegate(dlgt);

	GameNetwork()->RegisterEventHandler(MSG_ACCOUNT_S2C::CMD_S2CServerList, &test, &DlgtTest::scCmdLoginSuccess);

	GameNetwork()->Init();
	//GameNetwork()->Connect("192.168.1.245",8081);
	//GameNetwork()->Connect("221.130.10.112",30003);
	GameNetwork()->Connect("192.168.1.180",30001);
	//GameNetwork()->Connect("221.130.10.112",30001);

	GetPacketMgr()->init(GameNetwork());
	GetPacketMgr()->sendC2SLoginPacket("hongzb","123456","-2","Win32");
	//GetPacketMgr()->sendC2SPlayerListReqPacket("5F19E5E739FC27253468142BFE920D47");
	

	/*char tmp[3]={0,29,0};
	int seed=0x151EC118;
	GameNetwork()->encrypt(tmp,0,3,seed);
	YON_DEBUG("%u,%u,%u\r\n",tmp[0],tmp[1],tmp[2]);*/

#if 0
	CCmdPacket writePak;
	writePak.BeginWrite();
	YON_DEBUG("len:%d\r\n",writePak.GetDataSize());
	writePak.WriteFloat(3.2f);
	YON_DEBUG("len:%d\r\n",writePak.GetDataSize());
	writePak.WriteInt32(10);
	YON_DEBUG("len:%d\r\n",writePak.GetDataSize());
	writePak.WriteShort(8);
	YON_DEBUG("len:%d\r\n",writePak.GetDataSize());
	writePak.WriteString("Hello");
	YON_DEBUG("len:%d\r\n",writePak.GetDataSize());

	CCmdPacket readPak;
	readPak.BeginRead(writePak.GetData(),writePak.GetDataSize());
	float fv;
	int iv;
	short sv;
	core::stringc strv;
	int maxlen=10;
	strv.reserve(maxlen);
	memset(strv.pointer(), 0, maxlen );

	readPak.ReadFloat(&fv);
	readPak.ReadInt32(&iv);
	readPak.ReadShort(&sv);
	YON_DEBUG("%.2f,%d,%d\r\n",fv,iv,sv);
	readPak.ReadString(strv.pointer());
	strv.repair();
	YON_DEBUG("%s\r\n",strv.c_str());
#endif
	
	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
u32 last=0;
void drawFrame(){

	GameNetwork()->Update();

	if(last==0||timer->getRealTime()-last>30000)
	{
		//GetPacketMgr()->sendC2SSendSomeMessage();
		last=timer->getRealTime();
	}

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,SEND:%s,RECV:%s,POOL:%u/%u",videoDriver->getFPS(),GameNetwork()->getSendSpeed().c_str(),GameNetwork()->getRecvSpeed().c_str(),GameNetwork()->getPoolSize(),GameNetwork()->getPoolCapacity()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	GameNetwork()->Close();
	GameNetwork()->ReleaseInstance();
	GetPacketMgr()->ReleaseInstance();
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}