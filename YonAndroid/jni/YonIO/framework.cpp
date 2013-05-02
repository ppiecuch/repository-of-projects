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
II18NManager* i18nManager=NULL;
ITimer* timer=NULL;

SMaterial pointMaterial;
SDynamicShap2D pointShap;

ISceneNode* cubeModel=NULL;
ISceneNode* weedModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
f32 factor=1.1f;

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
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
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
class App
{
public:
	typedef CDelegate1<void, const core::stringc&> Callback;

	void debug(const core::stringc& str)
	{
		YON_DEBUG("%s\r\n",str.c_str());
	}

	static void info(const core::stringc& str)
	{
		YON_INFO("%s\r\n",str.c_str());
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
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	//fs->addWorkingDirectory("../media/",true);
	fs->addWorkingDirectory("../Yon/");
	//fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/",true);
#endif

	//Logger->debug("%s\n",fs->getAbsolutePath(fs->getWorkingDirectory()).c_str());

	pointMaterial.PolygonMode=video::ENUM_POLYGON_MODE_POINT;

	core::array<S2DVertex>& vertices=pointShap.getVertexArray();
	core::array<u16>& indices=pointShap.getIndexArray();

	S2DVertex v1(100,100,0,0,video::COLOR_WHITE);
	S2DVertex v2(100,150,0,0,video::COLOR_WHITE);
	S2DVertex v3(150,100,0,0,video::COLOR_WHITE);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

#if 0
	Logger->debug("%s\n",fs->getResourcePath("test.png").c_str());
	Logger->debug("%s\n",fs->getResourcePath("Android.mk").c_str());
	Logger->debug("%s\n",fs->getResourcePath("nothing.png").c_str());
	Logger->debug("%s\n",fs->getResourcePath("D:\\Development\\Software\\android-ndk-r7\\samples\\Irrlicht\\Examples\\nothing.png").c_str());

	Logger->debug("%s\n",fs->getAbsolutePath("test.png").c_str());
	Logger->debug("%s\n",fs->getAbsolutePath("D:\\Development\\Software\\android-ndk-r7\\samples\\Irrlicht\\Examples\\nothing.png").c_str());

	Logger->debug("%s\n",core::getParentName(fs->getResourcePath("tst/tst/tst/test.txt",true)).c_str());
	//Logger->debug("%d\n",_mkdir("d:/Development/Software/android-ndk-r7/samples/YonAndroid/jni/media/tst/tst/tst/"));
	//core::mkdirs(io::path("d:/Development/Software/android-ndk-r7/samples/YonAndroid/jni/media/tst/tst/tst/"));
	Logger->debug("success\n");

	//core::mkdirs(fs->getAbsolutePath(io::path("../media\\test1\\test2")).c_str());

	//Logger->debug("%s\n",core::getParentName(io::path("../media/fdsafs.txt")).c_str());

	core::ustring utf8_1("abcdefg");
	core::stringc origin_2=i18nManager->convert("你好UTF8",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	core::ustring utf8_2(origin_2.c_str());

	YON_DEBUG("%s(%d)\r\n",utf8_1.toStringc().c_str(),utf8_1.size());
	YON_DEBUG("%s(%d)\r\n",i18nManager->convert(utf8_2.toStringc().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str(),utf8_2.size());
#endif
#if 0
	u32 test=0xFFFFFFFF;
	YON_DEBUG("%u,%08X\r\n",test,(s32)test);
#elif 0
	App app;

	//创建委托。
	App::Callback callback = NULL;
	if(!callback.empty()) callback("1");

	core::IDelegate1<void, const core::stringc&>* d0=core::createDelegate(&app,&App::debug);
	core::IDelegate1<void, const core::stringc&>* d1=core::createDelegate(&App::info);

	//d0->drop();
	//d1->drop();

	callback += d0;
	if(!callback.empty()) callback("2");

	callback += d1;
	if(!callback.empty()) callback("3");

	callback -= d0;
	if(!callback.empty()) callback("4");

	callback -= d1;
	if(!callback.empty()) callback("5");

#elif 0

	u32 start=timer->getRealTime();
	for(u32 i=0;i<1000000;++i)
		YON_DEBUG_BREAK_IF(nearestPowerOf22(i)!=core::nearestPowerOf2(i));

	YON_DEBUG("%u\r\n",(timer->getRealTime()-start));


#elif 0

	//s32 s;
	//core::convertor<core::stringc,s32>::convert(core::stringc("134FF2"),s);
	//YON_DEBUG("%d\r\n",s);

	video::SColor c1(0xFF134ff2);
	YON_DEBUG("%s\r\n",c1.toHexString(true,false).c_str());
	//YON_DEBUG("%s\r\n",toHexString(0xFF134ff2,false,false).c_str());

	video::SColor c=video::SColor::fromHexString("#123456FF");
	YON_DEBUG("%08X\r\n",c.color);


#elif 0
	f32 f;
	core::convertor<s32,f32>::convert(1,f);
	YON_DEBUG("%.2f\r\n",f);

#elif 0
	core::stringc& str=video::COLOR_RED.toHexString(true,false);
	YON_DEBUG("%s\r\n",str.c_str());

#elif 0
	IWriteStream* ws=fs->createAndOpenWriteFileStream("d:/test.txt");
	ws->writeBool(true);
	ws->writeFloat(2.1f);
	ws->writeLong(3333);
	ws->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("d:/test.txt");
	Logger->debug("%d\n",rs->readBool());
	Logger->debug("%.2f\n",rs->readFloat());
	Logger->debug("%ld\n",rs->readLong());
	rs->drop();
#elif 0
	IWriteStream* ws=fs->createAndOpenWriteFileStream("tst/tst/tst/test.txt",true,ENUM_ENDIAN_MODE_BIG);
	ws->writeBool(true);
	ws->writeFloat(2.1f);
	ws->writeLong(3333);
	ws->writeString("helloworld");
	ws->writeString(core::stringc("helloChina"));
	ws->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("tst/tst/tst/test.txt",ENUM_ENDIAN_MODE_BIG);
	Logger->debug("%d\n",rs->readBool());
	Logger->debug("%.2f\n",rs->readFloat());
	Logger->debug("%ld\n",rs->readLong());
	Logger->debug("%s\r\n",rs->readString().c_str());
	Logger->debug("%s\r\n",rs->readString().c_str());
	//Logger->debug("%d\n",rs->readBool());
	//Logger->debug("%.2f\n",rs->readFloat());
	//Logger->debug("%ld\n",rs->readLong());
	rs->drop();
#elif 0
	IReadStream* rs=fs->createAndOpenReadFileStream("rotate_x_.dae");
	XMLReader* reader=fs->createXMLReader(rs);
	rs->drop();

	while(reader->read());
	reader->drop();
#elif 0
	IReadStream* stream=fs->createAndOpenReadFileStream("consume.xlb",io::ENUM_ENDIAN_MODE_BIG);
	core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();

	Logger->debug("rowCount:%d\r\n",rowCount);

	Logger->debug("%d\r\n",stream->readInt());
	//Logger->debug("%s\r\n",UTF8ToGB18030(stream->readString().c_str()).c_str());
	//Logger->debug("%s\r\n",EncodeConvertor::getInstance().GB18030ToUTF8(stream->readString().c_str()).c_str());
#ifdef YON_COMPILE_WITH_WIN32
	Logger->debug("%s\r\n",i18nManager->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	Logger->debug("%s\r\n",i18nManager->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	core::stringc test=i18nManager->convert("中华人民共和国",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	Logger->debug("%s\r\n",i18nManager->convert(test.c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
#elif defined(YON_COMPILE_WITH_ANDROID)
	Logger->debug("%s\r\n",stream->readString().c_str());
	Logger->debug("%s\r\n",stream->readString().c_str());
	core::stringc test2=i18nManager->convert("中华人民共和国",ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030);
	Logger->debug("%s\r\n",i18nManager->convert(test2.c_str(),ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8).c_str());
#endif
	//Logger->debug("%s\r\n",stream->readString().c_str());

	stream->drop();
#elif 0
	c8 in_utf8[] = 
	{0x20,0xe8,0xbf,0x99,0xe6,0x98,0xaf,0xe4,0xb8,0x80,0xe4,0xb8,0xaa,0x69,0x63,0x6f,
	0x6e,0x76,0xe7,0x9a,0x84,0xe6,0xb5,0x8b,0xe8,0xaf,0x95,0xe4,0xbe,0x8b,0xe7,0xa8,
	0x8b,0xef,0xbc,0x9a,0x0a,0xe2,0x80,0x9c,0xe7,0xba,0xb5,0xe6,0xb5,0xb7,0xe6,0x9d,
	0xaf,0xe2,0x80,0x9d,0xe4,0xb8,0x9c,0xe5,0x8d,0x97,0xe5,0xa4,0xa7,0xe5,0xad,0xa6,
	0xe7,0xac,0xac,0xe4,0xb8,0x89,0xe5,0xb1,0x8a,0xe5,0xb5,0x8c,0xe5,0x85,0xa5,0xe5,
	0xbc,0x8f,0xe7,0xb3,0xbb,0xe7,0xbb,0x9f,0xe8,0xae,0xbe,0xe8,0xae,0xa1,0xe9,0x82,
	0x80,0xe8,0xaf,0xb7,0xe8,0xb5,0x9b,0x00};    //UTF-8编码
	//Logger->debug("%s\r\n",i18nManager->convert("中华人民共和国",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8).c_str());
	//Logger->debug("%s\r\n",i18nManager->convert("涓崕浜烘皯鍏卞拰鍥",ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	//Logger->debug("%s\r\n",i18nManager->convert(in_utf8,ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());

#elif 0

	typedef core::map<s32,s32> TestMap;
	TestMap m;
	for(s32 i=0;i<100;++i)
		m.set(i,i);
	TestMap::Iterator iterator=m.getIterator();
	for(;!iterator.atEnd();++iterator)
		YON_DEBUG("%d->%d\r\n",iterator->getKey(),iterator->getValue());
#endif

	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);
	

	//videoDriver->setMaterial(pointMaterial);
	//videoDriver->drawShap(&pointShap);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}