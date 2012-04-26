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
				SOGLES1Parameters(HWND& hwnd,const core::dimension2du& windowSize):hWnd(hwnd),windowSize(windowSize){}
				HWND hWnd;
#elif defined(YON_COMPILE_WITH_ANDROID)
				SOGLES1Parameters(const core::dimension2du& windowSize):windowSize(windowSize){}
#endif//YON_COMPILE_WITH_WIN32

				//���ڳߴ�
				core::dimension2du windowSize;

			};
		}
	}
}

#endif//_YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_