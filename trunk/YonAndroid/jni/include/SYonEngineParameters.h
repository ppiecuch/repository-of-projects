#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"
#include "yonString.h"
#include "IEventReceiver.h"
#include "ICallback.h"
#include "ILogger.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <jni.h>
#endif

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
            appPath("."),
            resourcesPath("."),
			windowSize(core::dimension2du(800,600)),
			windowId(NULL),
			windowCaption(L"YonApplication"),
			pJNIEnv(NULL),
			pEventReceiver(NULL),
			pCallback(NULL),
			fpsLimit(0),
#ifdef YON_COMPILE_WITH_WIN32
			loggerAppender((MASK_APPENDER)(MASK_APPENDER_FILE|MASK_APPENDER_VS))
#elif defined(YON_COMPILE_WITH_ANDROID) || defined((YON_COMPILE_WITH_MACOSX))
			loggerAppender((MASK_APPENDER)(MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE))
#endif
			{}
		SYonEngineParameters(const SYonEngineParameters& params):
			appPath(params.appPath),
            resourcesPath(params.resourcesPath),
			windowSize(params.windowSize),
			windowId(params.windowId),
			windowCaption(params.windowCaption),
			pJNIEnv(params.pJNIEnv),
			pEventReceiver(params.pEventReceiver),
			pCallback(params.pCallback),
			fpsLimit(params.fpsLimit),
			loggerAppender(params.loggerAppender)
			{}

        //app·������Դ·��(ֻ����IOSƽ̨)
        io::path appPath;
        io::path resourcesPath;
		//���ڳߴ�
		core::dimension2du windowSize;
		//����ָ��
		mutable void* windowId;
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
		//Logger������ܵ�
		debug::MASK_APPENDER loggerAppender;
	};
}
#endif