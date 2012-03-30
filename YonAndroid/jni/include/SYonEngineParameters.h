#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"
#include "yonString.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <jni.h>
#endif

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
			windowSize(core::dimension2du(800,600)),
			windowId(NULL),
			windowCaption(L"YonApplication")
			{}
		SYonEngineParameters(const SYonEngineParameters& params):
			windowSize(params.windowSize),
			windowId(params.windowId),
			windowCaption(params.windowCaption),
			pJNIEnv(params.pJNIEnv)
			{}

		//窗口尺寸
		core::dimension2du windowSize;
		//窗口指针
		void* windowId;
		//窗口标题
		core::stringw windowCaption;
		//JNIEnv接口
		void *pJNIEnv;
	};
}
#endif