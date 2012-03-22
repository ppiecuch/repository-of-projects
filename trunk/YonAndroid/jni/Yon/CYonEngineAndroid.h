#ifndef _YON_PLATFORM_CXCENGINEANDROID_H_
#define _YON_PLATFORM_CXCENGINEANDROID_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include "IYonEngine.h"
#include "SYonEngineParameters.h"
#include "SOGLES1Parameters.h"
#include "COGLES1Driver.h"
#include "CSceneManager.h"

namespace yon{
	namespace platform{

		class CYonEngineAndroid:public IYonEngine{
		public:
			CYonEngineAndroid(const yon::SYonEngineParameters& params);
			~CYonEngineAndroid();

			virtual video::IVideoDriver* getVideoDriver(){return  m_videoDriver;}
			virtual scene::ISceneManager* getSceneManager(){return m_sceneManager;}
			virtual bool run();

			//virtual yon::ITimer* getTimer();

		protected:
			virtual void createDriver();
		private:
			//参数结构体
			yon::SYonEngineParameters m_params;

			video::IVideoDriver* m_videoDriver;
			scene::ISceneManager* m_sceneManager;

			bool m_bClose;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CXCENGINEWIN32_H_
