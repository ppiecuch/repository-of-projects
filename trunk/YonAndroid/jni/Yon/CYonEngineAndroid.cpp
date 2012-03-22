#include "config.h"

#ifdef YON_COMPILE_WITH_ANDROID

#include "CYonEngineAndroid.h"
#include "CSceneManager.h"

#include "ILogger.h"

namespace yon{
	namespace platform{


		CYonEngineAndroid::CYonEngineAndroid(const yon::SYonEngineParameters& params):
			m_videoDriver(NULL),m_sceneManager(new scene::CSceneManager()),
			m_params(params),m_bClose(false)
		{
			//初始化视频驱动器
			createDriver();
		}
		CYonEngineAndroid::~CYonEngineAndroid(){
			if(m_videoDriver!=NULL)
				m_videoDriver->drop();
			m_sceneManager->drop();
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Destroy CYonEngineAndroid");
			if(Logger->drop()){
				Logger=NULL;
			}
		}

		bool CYonEngineAndroid::run(){
			return !m_bClose;
		}

		void CYonEngineAndroid::createDriver(){

#ifdef YON_VIDEO_MODE_OGLES1
			yon::video::ogles1::SOGLES1Parameters params(m_params.windowSize);
			m_videoDriver=new yon::video::ogles1::COGLES1Driver(params);
#endif //Yon_VIDEO_MODE_OGLES1
		}
		//yon::ITimer* yon::platform::CYonEngineAndroid::getTimer(){return NULL;}
	}//platform
}//yon

#endif//YON_COMPILE_WITH_ANDROID