#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <jni.h>
#include "CYonEngineAndroid.h"
#include "CSceneManager.h"
#include "CGraphicsAdapter.h"

#include "ILogger.h"

namespace yon{
namespace platform{

	#define JNI_VERSION_1_1 0x00010001
	#define JNI_VERSION_1_2 0x00010002
	#define JNI_VERSION_1_4 0x00010004
	#define JNI_VERSION_1_5 0x00010005
	#define JNI_VERSION_1_6 0x00010006

	void CYonEngineAndroid::checkJNIVersion(){
		JNIEnv* evn=(JNIEnv*)m_params.pJNIEnv;
		jint version= evn->GetVersion();
		switch(version){
			case JNI_VERSION_1_1:
				m_pfInfo.jniVersion="1.1";
				break;
			case JNI_VERSION_1_2:
				m_pfInfo.jniVersion="1.2";
				break;
			case JNI_VERSION_1_4:
				m_pfInfo.jniVersion="1.4";
				break;
			case JNI_VERSION_1_5:
				m_pfInfo.jniVersion="1.5";
				break;
			case JNI_VERSION_1_6:
				m_pfInfo.jniVersion="1.6";
				break;
			default:
				m_pfInfo.jniVersion="unknown";
				break;
		}
	}


	CYonEngineAndroid::CYonEngineAndroid(const yon::SYonEngineParameters& params)
		:m_pVideoDriver(NULL),m_pSceneManager(NULL),m_pFileSystem(NULL),
		m_pUserListener(params.pEventReceiver),m_pGraphicsAdapter(NULL),
		m_pAudioDriver(NULL),
		m_params(params),m_bClose(false),m_bResized(true)
	{
		Logger->debug("start instance CYonEngineAndroid\n");
		//检测jni版本
		checkJNIVersion();
		Logger->info("JNI Version:%s\n",m_pfInfo.jniVersion.c_str());

		//初始化计时器
		m_pTimer=createTimer();

		//初始化文件系统
		m_pFileSystem=io::createFileSystem();

		//初始化场景管理器
		m_pSceneManager=scene::createSceneManager();

		//初始化视频驱动器
		createDriver();

		//初始化Graphics适配器
		m_pGraphicsAdapter=scene::createGraphicsAdapter(m_pVideoDriver,m_pSceneManager);

		//初始化声音驱动器
		m_pAudioDriver=audio::createAudioDriver(m_pFileSystem);

		//启动计时器
		m_pTimer->start();
	}
	CYonEngineAndroid::~CYonEngineAndroid(){
	m_pAudioDriver->drop();
	m_pGraphicsAdapter->drop();
	m_pVideoDriver->drop();
	m_pSceneManager->drop();
	m_pFileSystem->drop();
	m_pTimer->drop();
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineAndroid");
		if(video::DEFAULT_MATERIAL->drop()){
			video::DEFAULT_MATERIAL=NULL;
		}
		if(Logger->drop()){
			Logger=NULL;
		}
	}

	bool CYonEngineAndroid::run(){
		m_pTimer->tick();
		if(!m_bClose)
			resizeIfNecessary();
		return !m_bClose;
	}
	void CYonEngineAndroid::onResize(u32 w,u32 h){
		m_bResized=true;
		m_params.windowSize.w=w;
		m_params.windowSize.h=h;
		Logger->debug("CYonEngineAndroid::onResize(%d,%d)",w,h);
	}

	void CYonEngineAndroid::resizeIfNecessary()
	{
		if (!m_bResized)
			return;

		m_pVideoDriver->onResize(m_params.windowSize);
		m_pSceneManager->onResize(m_params.windowSize);
		m_bResized = false;
		
		Logger->debug("CYonEngineAndroid::resizeIfNecessary\n");
	}

	bool CYonEngineAndroid::postEventFromUser(const event::SEvent& event){
		bool absorbed = false;

		if (m_pUserListener)
			absorbed = m_pUserListener->onEvent(event);

		//TODO GUI
		//if (!absorbed && GUIEnvironment)
		//	absorbed = GUIEnvironment->postEventFromUser(event);

		if (!absorbed && m_pSceneManager)
			absorbed = m_pSceneManager->postEventFromUser(event);

		return absorbed;
	}

	void CYonEngineAndroid::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
		video::ogles1::SOGLES1Parameters params(m_params.windowSize,m_params.fpsLimit);
		m_pVideoDriver=new video::ogles1::COGLES1Driver(params,m_pFileSystem,m_pTimer,m_pSceneManager->getGeometryFactory());
#endif //Yon_VIDEO_MODE_OGLES1
	}
	//yon::ITimer* yon::platform::CYonEngineAndroid::getTimer(){return NULL;}
}//platform
}//yon

#endif//YON_COMPILE_WITH_ANDROID