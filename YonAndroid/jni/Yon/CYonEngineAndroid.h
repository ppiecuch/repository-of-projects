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
			CYonEngineAndroid(const SYonEngineParameters& params);
			~CYonEngineAndroid();

			virtual video::IVideoDriver* getVideoDriver(){return  m_videoDriver;}
			virtual scene::ISceneManager* getSceneManager(){return m_sceneManager;}
			virtual bool run();

			virtual void onResize(u32 w,u32 h);

			//virtual yon::ITimer* getTimer();

		protected:
			virtual void createDriver();
		private:
			void resizeIfNecessary();

			//参数结构体
			yon::SYonEngineParameters m_params;

			video::IVideoDriver* m_videoDriver;
			scene::ISceneManager* m_sceneManager;

			bool m_bClose;
			bool m_bResized;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CXCENGINEWIN32_H_
