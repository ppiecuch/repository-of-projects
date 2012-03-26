#ifndef _YON_PLATFORM_CYONENGINEWIN32_H_
#define _YON_PLATFORM_CYONENGINEWIN32_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32
////����Window������Ҫ��ͷ�ļ�
#include <windows.h>

#include "IYonEngine.h"
#include "SYonEngineParameters.h"
#include "SOGLES1Parameters.h"
#include "COGLES1Driver.h"
#include "ISceneManager.h"
#include "IFileSystem.h"

namespace yon{

	namespace scene{
		ISceneManager* createSceneManager();
	}

	namespace io
	{
		IFileSystem* createFileSystem();
	}


	namespace platform{
		
		class CYonEngineWin32:public IYonEngine{
		public:
			CYonEngineWin32(const yon::SYonEngineParameters& params);
			~CYonEngineWin32();

			virtual video::IVideoDriver* getVideoDriver() const{return  m_videoDriver;}
			virtual scene::ISceneManager* getSceneManager() const{return m_sceneManager;}
			virtual io::IFileSystem* getFileSystem() const{return m_fileSystem;}

			virtual bool run();

			virtual void onResize(u32 w,u32 h);

			//virtual ITimer* getTimer();

			const HWND& getHWND() const{
				return m_hWnd;
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

			video::IVideoDriver* m_videoDriver;
			scene::ISceneManager* m_sceneManager;
			io::IFileSystem* m_fileSystem;

			bool m_bClose;
			bool m_bResized;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CYONENGINEWIN32_H_