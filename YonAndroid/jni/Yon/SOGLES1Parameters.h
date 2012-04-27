#ifndef _YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_
#define _YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_

#include "yonConfig.h"
#include "dimension2d.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#endif//YON_COMPILE_WITH_WIN32

namespace yon{
	namespace video{
		namespace ogles1{

			struct SOGLES1Parameters{

#ifdef YON_COMPILE_WITH_WIN32
				SOGLES1Parameters(HWND& hwnd,const core::dimension2du& windowSize,u32 fpsLimit)
					:hWnd(hwnd),windowSize(windowSize),fpsLimit(fpsLimit){}
				HWND hWnd;
#elif defined(YON_COMPILE_WITH_ANDROID)
				SOGLES1Parameters(const core::dimension2du& windowSize,u32 fpsLimit)
					:windowSize(windowSize),fpsLimit(fpsLimit){}
#endif//YON_COMPILE_WITH_WIN32

				//窗口尺寸
				core::dimension2du windowSize;
				//FPS上限(为0表示不作限制)
				u32 fpsLimit;
			};
		}
	}
}

#endif