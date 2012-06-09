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
				logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
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
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

	Logger->debug("%s\n",fs->getAbsolutePath(fs->getWorkingDirectory()).c_str());

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
	IWriteStream* ws=fs->createAndOpenWriteFileStream("/tst/tst/tst/test.txt",true,ENUM_ENDIAN_MODE_BIG);
	ws->writeBool(true);
	ws->writeFloat(2.1f);
	ws->writeLong(3333);
	ws->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("/tst/tst/tst/test.txt",ENUM_ENDIAN_MODE_BIG);
	Logger->debug("%d\n",rs->readBool());
	Logger->debug("%.2f\n",rs->readFloat());
	Logger->debug("%ld\n",rs->readLong());
	//Logger->debug("%d\n",rs->readBool());
	//Logger->debug("%.2f\n",rs->readFloat());
	//Logger->debug("%ld\n",rs->readLong());
	rs->drop();
#else
	IReadStream* rs=fs->createAndOpenReadFileStream("video_autodetect.xml");
	XMLReader* reader=fs->createXMLReader(rs);
	rs->drop();

	while(reader->read());
	reader->drop();
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