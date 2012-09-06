#ifndef _YON_PLATFORM_CYONENGINEWIN32_H_
#define _YON_PLATFORM_CYONENGINEWIN32_H_

#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_WIN32
////����Window������Ҫ��ͷ�ļ�
#include <windows.h>

#include "IYonEngine.h"
#include "SYonEngineParameters.h"
#include "SOGLES1Parameters.h"
#include "COGLES1Driver.h"
#include "ISceneManager.h"
#include "IFileSystem.h"
#include "ICursorControl.h"

namespace yon{
	namespace platform{
		
		class CYonEngineWin32:public IYonEngine{
		public:
			CYonEngineWin32(const yon::SYonEngineParameters& params);
			~CYonEngineWin32();

			virtual video::IVideoDriver* getVideoDriver() const{return  m_pVideoDriver;}
			virtual scene::ISceneManager* getSceneManager() const{return m_pSceneManager;}
			virtual scene::IGraphicsAdapter* getGraphicsAdapter() const{return m_pGraphicsAdapter;}
			virtual scene::IGraphicsAdapter* getGraphicsAdapterWindow() const{return m_pGraphicsAdapterWindow;}
			virtual io::IFileSystem* getFileSystem() const{return m_pFileSystem;}
			virtual audio::IAudioDriver* getAudioDriver() const{return m_pAudioDriver;}
			virtual IRandomizer* getRandomizer(){return m_pRandomizer;}
			virtual i18n::II18NManager* getI18NManager(){return m_pI18nManager;}

			virtual bool available(){return m_bAvailable;}

			virtual bool run();

			virtual void onResize(u32 w,u32 h);

			virtual bool callback(const platform::SCallback& cb);
			virtual bool postEventFromUser(const event::SEvent& event);

			virtual ITimer* getTimer(){
				return m_pTimer;
			}

			const HWND& getHWND() const{
				return m_hWnd;
			}

			virtual const SYonEngineParameters& getCreateParameters(){
				return m_params;
			}
		protected:
			virtual void createDriver();
		private:
			//��ʼ�����ڣ��ɹ��򷵻�true��ʧ���򷵻�false
			bool initWindow(const yon::SYonEngineParameters& params);
			void resizeIfNecessary();
			//���ھ��
			//Microsoft Windows ���л�����ͨ����Ӧ�ó����е�ÿ������Ϳؼ�����һ��������� hWnd������ʶ���ǡ�
			//hWnd ��������Windows API���á���� Windows ���л���������Ҫ����ڵ� hWnd ��Ϊ������
			//ע�� ���ڸ�����ֵ�ڳ�������ʱ���Ըı䣬���Բ�Ҫ�� hWnd �洢�ڱ����С�
			HWND m_hWnd;
			//�󶨵��ⲿ����
			bool m_bExternalWindow;
			//�����ṹ��
			yon::SYonEngineParameters m_params;

			video::IVideoDriver* m_pVideoDriver;
			scene::ISceneManager* m_pSceneManager;
			scene::IGraphicsAdapter* m_pGraphicsAdapter;
			scene::IGraphicsAdapter* m_pGraphicsAdapterWindow;
			io::IFileSystem* m_pFileSystem;
			audio::IAudioDriver* m_pAudioDriver;
			ITimer* m_pTimer;
			IRandomizer* m_pRandomizer;
			ICursorControl* m_pCursorControl;
			i18n::II18NManager* m_pI18nManager;

			//TODO�Ƿ��д��ڱ�Ҫ��������
			event::IEventReceiver* m_pUserListener;
			platform::ICallback* m_pCallback;

			bool m_bClose;
			bool m_bResized;
			bool m_bAvailable;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CYONENGINEWIN32_H_