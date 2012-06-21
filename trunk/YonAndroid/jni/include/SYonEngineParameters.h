#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"
#include "yonString.h"
#include "IEventReceiver.h"
#include "ICallback.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <jni.h>
#endif

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
			windowSize(core::dimension2du(800,600)),
			windowId(NULL),
			windowCaption(L"YonApplication"),
			pJNIEnv(NULL),
			pEventReceiver(NULL),
			pCallback(NULL),
			fpsLimit(0)
			{}
		SYonEngineParameters(const SYonEngineParameters& params):
			windowSize(params.windowSize),
			windowId(params.windowId),
			windowCaption(params.windowCaption),
			pJNIEnv(params.pJNIEnv),
			pEventReceiver(params.pEventReceiver),
			pCallback(params.pCallback),
			fpsLimit(params.fpsLimit)
			{}

		//���ڳߴ�
		core::dimension2du windowSize;
		//����ָ��
		void* windowId;
		//���ڱ���
		core::stringw windowCaption;
		//JNIEnv�ӿ�
		void *pJNIEnv;
		//�Զ����¼�������
		event::IEventReceiver* pEventReceiver;
		//�Զ���ص�
		platform::ICallback* pCallback;
		//FPS����(Ĭ��Ϊ0��ʾ��������)
		u32 fpsLimit;
	};
}
#endif