#ifndef _YON_PLATFORM_CYONPLATFORMWIN32_H_
#define _YON_PLATFORM_CYONPLATFORMWIN32_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32
////����Window������Ҫ��ͷ�ļ�
#include <windows.h>

#include "IYonPlatform.h"
#include "SYonEngineParameters.h"

namespace yon{
	namespace platform{
		
		class CYonPlatformWin32:public IYonPlatform{
		public:
			CYonPlatformWin32(const yon::SYonEngineParameters& params);
			~CYonPlatformWin32();
			//virtual yon::ITimer* getTimer();

			const HWND& getHWND() const{
				return m_hWnd;
			}
		private:
			//��ʼ�����ڣ��ɹ��򷵻�true��ʧ���򷵻�false
			bool initWindow(const yon::SYonEngineParameters& params);

			//���ھ��
			//Microsoft Windows ���л�����ͨ����Ӧ�ó����е�ÿ������Ϳؼ�����һ��������� hWnd������ʶ���ǡ�
			//hWnd ��������Windows API���á���� Windows ���л���������Ҫ����ڵ� hWnd ��Ϊ������
			//ע�� ���ڸ�����ֵ�ڳ�������ʱ���Ըı䣬���Բ�Ҫ�� hWnd �洢�ڱ����С�
			HWND m_hWnd;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CYONPLATFORMWIN32_H_