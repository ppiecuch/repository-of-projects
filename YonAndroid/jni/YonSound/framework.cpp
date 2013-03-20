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
ITimer* timer=NULL;

ISceneNode* cubeModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
video::ITexture* rtt=NULL;
f32 factor=1.1f;

const c8* names[]={"xyg.ogg","110.ogg","111.ogg","112.ogg","113.ogg","114.ogg","115.ogg","116.ogg","117.ogg","118.ogg","119.ogg"};
typedef core::map<core::stringc,ISound*> SoundMap;
SoundMap soundPool;

#define MAX_IN_POOL 7

void playSound(s32 index)
{
	core::stringc name=names[index];
	SoundMap::Node* node=soundPool.find(name);
	if(node)
	{
		ISound* sound=node->getValue();
		//if(sound->isPlaying())
		//»ò
		if(sound->isStopped()==false)
			YON_DEBUG("sound playing:%s\r\n",name.c_str());
		else
		{
			//sound->setSecondOffset(0);
			sound->play();
			sound->setLooping(false);
		}
	}
	else
	{
		YON_DEBUG("sound size:%u\r\n",soundPool.size());
		if(soundPool.size()>=MAX_IN_POOL)
		{
			SoundMap::Iterator it=soundPool.getIterator();
			for(;!it.atEnd();++it)
			{
				ISound* sound=it->getValue();
				if(sound->isStopped())
				{
					YON_DEBUG("remove sound:%s\r\n",it->getKey().c_str());
					soundPool.remove(it->getKey());
					audioDriver->removeSound(sound);
					break;
				}
				//else
				//	YON_DEBUG("sound %s not stop\r\n",it->getKey().c_str());
			}
		}
		if(soundPool.size()<=MAX_IN_POOL)
		{
			ISound* sound=audioDriver->getSound(name);
			soundPool.insert(name,sound);
			sound->play();
			sound->setLooping(false);
		}
		else
			YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("currently has no free space to play sound:%s,ignore it!",name.c_str()).c_str());
	}
}
void play(){
	s32 start=randomizer::rand(0,9);
	s32 num=randomizer::rand(1,10-start);
	YON_INFO("play:start:%d,num:%d,from:[%s],to:[%s]\r\n",start,num,names[start],names[start+num-1]);
	for(s32 i=start;i<start+num;++i)
		playSound(i);
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
				play();
				//{
				//	ISound* sound=(ISound*)soundPool["xyg.ogg"];
				//	sound->stop();
				//}
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			}
			break;
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.touchInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				play();
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
			break;
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(evt.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_DOZE:
				YON_INFO("ENUM_SYSTEM_INPUT_TYPE_DOZE\r\n");
				return true;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				YON_INFO("ENUM_SYSTEM_INPUT_TYPE_WAKE\r\n");
				return true;
			}
			break;
		default:
			YON_WARN(YON_LOG_WARN_FORMAT,"unexpect event type:%d",evt.type);
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
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/sound");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("media/sound");
#endif

	//YON_DEBUG("maxMixed:%d \r\n",audioDriver->getMaxMixedSound());
	u32 start=timer->getRealTime();
	ISound* sound=audioDriver->getSound("xyg.ogg");
	
	soundPool.insert(names[0],sound);
	sound->play();
	sound->setLooping(true);
	YON_DEBUG("use time:%u \r\n",timer->getRealTime()-start);

	//PROFILE_REGISTER_FRAME();
	//PROFILE_START_CALL(PROFILE_ID_1,Driver->begin);
	//sound1=audioDriver->getSound("bg.ogg");
	//sound2=audioDriver->getSound("202.ogg");
	
	//sound2=audioDriver->getSound("helloworld.wav");
	//audioDriver->grabSound(sound);
	//audioDriver->dropSound(sound);
	//sound1->setLooping(true);
	//sound2->setLooping(false);
	//sound1->play();
	//sound1->setGain(0.2f);

	//sound2->setLooping(false);
	//sound2->play();
	//sound2->setGain(0.5f);
	//sound=audioDriver->getSound("helloworld.wav");
	//sound->play();
	//PROFILE_END_CALL(PROFILE_ID_1);
	//PROFILE_REGISTER_FRAME();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	audioDriver->update();

	videoDriver->begin();

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,maxMixed:%d",videoDriver->getFPS(),audioDriver->getMaxMixedSound()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	SoundMap::Iterator it=soundPool.getIterator();
	for(s32 index=1;!it.atEnd();++it,++index)
	{
		Logger->drawString(videoDriver,core::stringc("sound:%s",it->getKey().c_str()),core::position2di(10,10*index),COLOR_GREEN);
	}

	videoDriver->end();
}
void destroy(){
	//PROFILE_REPORT();
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}