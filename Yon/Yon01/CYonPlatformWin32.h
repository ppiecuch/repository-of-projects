#ifndef _YON_PLATFORM_CYONPLATFORMWIN32_H_
#define _YON_PLATFORM_CYONPLATFORMWIN32_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32
////加载Window程序需要的头文件
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
			//初始化窗口，成功则返回true，失败则返回false
			bool initWindow(const yon::SYonEngineParameters& params);

			//窗口句柄
			//Microsoft Windows 运行环境，通过给应用程序中的每个窗体和控件分配一个句柄（或 hWnd）来标识它们。
			//hWnd 属性用于Windows API调用。许多 Windows 运行环境函数需要活动窗口的 hWnd 作为参数。
			//注意 由于该属性值在程序运行时可以改变，绝对不要将 hWnd 存储在变量中。
			HWND m_hWnd;
		};
	}
}
#endif //YON_COMPILE_WITH_WIN32
#endif //_YON_PLATFORM_CYONPLATFORMWIN32_H_