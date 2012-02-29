#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"
#include "yonString.h"

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
			windowSize(core::dimension2du(800,600)),
			windowId(NULL),
			windowCaption(L"YonApplication")
			{}

		//窗口尺寸
		core::dimension2du windowSize;
		//窗口指针
		void* windowId;
		//窗口标题
		core::stringw windowCaption;
	};
}
#endif