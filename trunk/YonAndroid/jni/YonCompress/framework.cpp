#include <string>
#include <vector>
using namespace std;

#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_ANDROID

#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"

bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,void* context,bool succeeded) {
	YON_DEBUG("Dump path: %s\n", descriptor.path());
	return succeeded;
}

void Crash() {
	volatile int* a = reinterpret_cast<volatile int*>(NULL);
	*a = 1;
}

#endif

#include "framework.h"

ENUM_ENDIAN_MODE getEndian(){
	static bool ret=(bool)(*(unsigned short *)"\0\xff" < 0x100);
	return  ret?ENUM_ENDIAN_MODE_BIG:ENUM_ENDIAN_MODE_LITTLE;
}


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

ISceneNode* cubeModel=NULL;
f32 factor=1.1f;
core::position2di ps[4];

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
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_DOZE;
					getEngine()->postEventFromUser(evt);
				}
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				{
					SEvent evt;
					evt.type=ENUM_EVENT_TYPE_SYSTEM;
					evt.systemInput.type=ENUM_SYSTEM_INPUT_TYPE_WAKE;
					getEngine()->postEventFromUser(evt);
				}
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
#ifdef YON_COMPILE_WITH_ANDROID
				{
					google_breakpad::MinidumpDescriptor descriptor(fs->getResourcePath(".").c_str());
					google_breakpad::ExceptionHandler eh(descriptor, NULL, DumpCallback,NULL, true, -1);
					Crash();
				}
#endif
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
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
	params.fpsLimit=0;
	params.appPath=appPath;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	if(engine->available()==false)
		return false;
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory* animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	//randomizer=engine->getRandomizer();

	//videoDriver->setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,true);

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media",true);
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media",true);
#endif

#define TO_PS(x,y,w,h) \
	ps[0].set(x,y+h); \
	ps[1].set(x,y); \
	ps[2].set(x+w,y); \
	ps[3].set(x+w,y+h);

	TO_PS(10,10,1024,1024)

	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin();

	sceneMgr->render(videoDriver);

	static core::rectf r(0,0,1,1);
	gfAdapter->clearZ(1000);
	ITexture* texture=videoDriver->getTexture("506_4444.pvr.ccz");
	//gfAdapter->drawFill(texture,r,ps,ENUM_TRANS_NONE,0xFF00FF00);
	gfAdapter->drawRegion(texture,r,ps,ENUM_TRANS_NONE,true);
	gfAdapter->render();


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