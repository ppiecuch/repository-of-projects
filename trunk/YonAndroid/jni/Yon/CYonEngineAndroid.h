#ifndef _YON_PLATFORM_CXCENGINEANDROID_H_
#define _YON_PLATFORM_CXCENGINEANDROID_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include "IYonEngine.h"
#include "SYonEngineParameters.h"
#include "SOGLES1Parameters.h"
#include "COGLES1Driver.h"
#include "ISceneManager.h"
#include "IFileSystem.h"

namespace yon{
	namespace platform{

		class CYonEngineAndroid:public IYonEngine{
		public:
			CYonEngineAndroid(const SYonEngineParameters& params);
			~CYonEngineAndroid();

			virtual video::IVideoDriver* getVideoDriver() const{return  m_pVideoDriver;}
			virtual scene::ISceneManager* getSceneManager() const{return m_pSceneManager;}
			virtual io::IFileSystem* getFileSystem() const{return m_pFileSystem;}

			virtual bool run();

			virtual void onResize(u32 w,u32 h);

			//virtual yon::ITimer* getTimer();

		protected:
			virtual void createDriver();
		private:
			void resizeIfNecessary();

			//参数结构体
			yon::SYonEngineParameters m_params;

			video::IVideoDriver* m_pVideoDriver;
			scene::ISceneManager* m_pSceneManager;
			io::IFileSystem* m_pFileSystem;

			bool m_bClose;
			bool m_bResized;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CXCENGINEWIN32_H_
