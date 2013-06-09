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
ITimer* timer=NULL;

class DecryptReadStreamCreator : public io::IReadStreamCreator{
private:
	const static s32 HEAD_SIZE=5;
public:
	virtual IReadStream* createAndOpenReadFileStream(const io::path& filename,ENUM_ENDIAN_MODE mode){
		//备份构造者
		io::IReadStreamCreator* rsc=fs->getActivedReadFileStreamCreator();
		rsc->grab();
		fs->activeReadFileStreamCreator(NULL);

		io::IReadStream* rs=fs->createAndOpenReadFileStream(filename,mode);
		if(!rs)
			return NULL;

		YON_DEBUG_BREAK_IF(rs->getSize()<5);

		s32 ver=rs->readInt();
		u8 key=rs->readUnsignedByte();
		u32 len=rs->getSize()-HEAD_SIZE;
		u8* arr=new u8[len];

		rs->read(arr,len);
		rs->drop();

		//core::stringc str;
		for(u32 i=0;i<len;++i)
		{
			u32 pos = i+HEAD_SIZE;
			u8 c=arr[i];

			if (pos&0x01)
			{
				//奇
				key ^= pos << 1 + 1;
			}else
			{
				//偶
				key ^= pos << 2 + 1;
			}
			arr[i]=(u8)key^c;
			//YON_DEBUG("%u\r\n",arr[i]);
			//str.append(core::stringc("%u,",arr[i]));
		}
		//YON_DEBUG("str:%s\r\n",str.c_str());

		//恢复构造者
		fs->activeReadFileStreamCreator(rsc);
		rsc->drop();
		return fs->createAndOpenReadMemoryStream(filename,arr,len,true,mode);
	}
};

#define VERSION0 0
#define VERSION1 1
#define VERSION2 2

bool decrypt(int version,io::IReadStream* src,core::array<u8>& dest)
{
	switch(version)
	{
	case VERSION0:
		{
			core::stringc str;
			while(src->getPos()<src->getSize()){
				u8 c=src->readUnsignedByte();
				dest.push_back(c);
				str.append(core::stringc("%u,",dest.getLast()));
			}
			YON_DEBUG("str:%s\r\n",str.c_str());
			return true;
		}
	case VERSION1:
		{
			u8 key=src->readUnsignedByte();
			dest.set_used(src->getSize());
			dest.set_used(0);
			s32 index=0;
			while(src->getPos()<src->getSize()){
				u8 c=src->readUnsignedByte();
				dest.push_back(key^c);
			}
			return true;
		}
		break;
	case VERSION2:
		{
			s32 ver=src->readInt();
			u8 key=src->readUnsignedByte();
			dest.set_used(src->getSize());
			dest.set_used(0);
			s32 index=0;
			//core::stringc str;
			while(src->getPos()<src->getSize()){
				u32 pos = src->getPos();
				u8 c=src->readUnsignedByte();

				if (pos&0x01)
				{
					//奇
					key ^= pos << 1 + 1;
				}else
				{
					//偶
					key ^= pos << 2 + 1;
				}
				dest.push_back(key^c);
				//YON_DEBUG("%u\r\n",dest.getLast());
				//str.append(core::stringc("%u,",dest.getLast()));
			}
			//YON_DEBUG("str:%s\r\n",str.c_str());
			return true;
		}
		break;
	default:
		YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("%s has unexpect file header, skit it!",src->getPath().c_str()).c_str());
	}
	return false;
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
	timer=engine->getTimer();
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("..\\media\\encrypt");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/encrypt");
#endif

	//校验加密
#if 0
	io::IReadStream* png=fs->createAndOpenReadFileStream("luminance.png");
	io::IReadStream* ecp=fs->createAndOpenReadFileStream("luminance.ecp");

	core::array<u8> pa;
	core::array<u8> pe;

	decrypt(VERSION0,png,pa);
	decrypt(VERSION2,ecp,pe);

	png->drop();
	ecp->drop();

	//YON_DEBUG_BREAK_IF(pa.size()!=pe.size());

	for(u32 i=0;i<pa.size();++i)
	{
		YON_DEBUG_BREAK_IF(pa[i]!=pe[i]);
	}
#elif 1
	io::IReadStreamCreator* rsc=new DecryptReadStreamCreator();
	fs->activeReadFileStreamCreator(rsc);
	videoDriver->getTexture("luminance.ecp");

	u32 start=timer->getRealTime();
	//streaming方式待支持
	//ISound* sound=audioDriver->getSound("1.ogg",true);
	ISound* sound=audioDriver->getSound("1.ogg");

	sound->play();
	sound->setLooping(true);
	YON_DEBUG("use time:%u \r\n",timer->getRealTime()-start);

	fs->activeReadFileStreamCreator(NULL);
	rsc->drop();
#else

	io::IReadStream* png=fs->createAndOpenReadFileStream("luminance.png");
	
	io::IReadStreamCreator* rsc=new DecryptReadStreamCreator();
	fs->activeReadFileStreamCreator(rsc);
	io::IReadStream* ecp=fs->createAndOpenReadFileStream("luminance.ecp");
	fs->activeReadFileStreamCreator(NULL);
	rsc->drop();

	core::array<u8> pa;
	core::array<u8> pe;

	decrypt(VERSION0,png,pa);
	decrypt(VERSION0,ecp,pe);

	png->drop();
	ecp->drop();

	//YON_DEBUG_BREAK_IF(pa.size()!=pe.size());

	for(u32 i=0;i<pa.size();++i)
	{
		YON_DEBUG_BREAK_IF(pa[i]!=pe[i]);
	}
#endif
	

	
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
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}