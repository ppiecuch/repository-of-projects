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
II18NManager* i18nManager=NULL;

IModel* cubeModel=NULL;
IModel* weedModel=NULL;
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

bool init(void *pJNIEnv,u32 width,u32 height){
	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pJNIEnv=pJNIEnv;
	//params.fpsLimit=10;
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
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../Yon/");
	fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("temp/");
#endif

	//Logger->debug("%s\n",fs->getAbsolutePath(fs->getWorkingDirectory()).c_str());

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
#endif
	//core::mkdirs(fs->getAbsolutePath(io::path("../media\\test1\\test2")).c_str());

	//Logger->debug("%s\n",core::getParentName(io::path("../media/fdsafs.txt")).c_str());

#if 0
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
	IReadStream* rs=fs->createAndOpenReadFileStream("video_autodetect.xml");
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
	//Logger->debug("%s\r\n",stream->readString().c_str());

	stream->drop();
#else
	Logger->debug("%s\r\n",i18nManager->convert("中华人民共和国",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8).c_str());
	Logger->debug("%s\r\n",i18nManager->convert("涓崕浜烘皯鍏卞拰鍥",ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
#endif

	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}