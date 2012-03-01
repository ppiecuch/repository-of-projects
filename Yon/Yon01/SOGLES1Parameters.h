#ifndef _YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_
#define _YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_

#include "config.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#endif//YON_COMPILE_WITH_WIN32

namespace yon{
	namespace video{
		namespace ogles1{

			struct SOGLES1Parameters{

#ifdef YON_COMPILE_WITH_WIN32
			SOGLES1Parameters(HWND& hwnd):hWnd(hwnd){}
			HWND hWnd;
#endif//YON_COMPILE_WITH_WIN32

			};
		}
	}
}

#endif//_YON_VIDEO_OGLES1_SOGLES1PARAMETERS_H_