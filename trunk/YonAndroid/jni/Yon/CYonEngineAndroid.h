#ifndef _YON_PLATFORM_CXCENGINEANDROID_H_
#define _YON_PLATFORM_CXCENGINEANDROID_H_

#include "yonConfig.h"

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
			virtual scene::IGraphicsAdapter* getGraphicsAdapter() const{return m_pGraphicsAdapter;}
			virtual io::IFileSystem* getFileSystem() const{return m_pFileSystem;}
			virtual audio::IAudioDriver* getAudioDriver() const{return m_pAudioDriver;}
			virtual IRandomizer* getRandomizer(){return m_pRandomizer;}
			virtual i18n::II18NManager* getI18NManager(){return m_pI18nManager;}

			virtual bool run();

			virtual void onResize(u32 w,u32 h);

			virtual bool callback(const platform::SCallback& cb);
			virtual bool postEventFromUser(const event::SEvent& event);

			virtual ITimer* getTimer(){
				return m_pTimer;
			}
			virtual const SYonEngineParameters& getCreateParameters(){
				return m_params;
			}
		protected:
			virtual void createDriver();
			void checkJNIVersion();
		private:
			void resizeIfNecessary();

			//参数结构体
			yon::SYonEngineParameters m_params;
			//平台相关信息
			SYonPlatformInfo m_pfInfo;

			video::IVideoDriver* m_pVideoDriver;
			scene::ISceneManager* m_pSceneManager;
			scene::IGraphicsAdapter* m_pGraphicsAdapter;
			io::IFileSystem* m_pFileSystem;
			audio::IAudioDriver* m_pAudioDriver;
			ITimer* m_pTimer;
			IRandomizer* m_pRandomizer;
			i18n::II18NManager* m_pI18nManager;

			event::IEventReceiver* m_pUserListener;
			platform::ICallback* m_pCallback;

			bool m_bClose;
			bool m_bResized;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CXCENGINEWIN32_H_
