#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"
#include "yonString.h"

#ifdef XC_COMPILE_WITH_ANDROID
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
			windowCaption(params.windowCaption)
			{}

		//���ڳߴ�
		core::dimension2du windowSize;
		//����ָ��
		void* windowId;
		//���ڱ���
		core::stringw windowCaption;
		//JNIEnv�ӿ�
		void *pJNIEnv;
	};
}
#endif