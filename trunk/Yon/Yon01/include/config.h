#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_VERSION_MAJOR 0 //���汾��
#define YON_VERSION_MINOR 1	//�Ӱ汾��

//����ƽ̨(Win32/Android/MacOSX)
#define YON_COMPILE_WITH_WIN32
//#define YON_COMPILE_WITH_ANDROID
//#define YON_COMPILE_WITH_MACOSX

//��Ƶģʽ(OpenGLES1/OpenGLES2)
#define  YON_VIDEO_MODE_OGLES1
//#define  YON_VIDEO_MODE_OGLES2

//DLL���뵼��
#ifdef YON01_EXPORTS
#define YON_API __declspec(dllexport)
#else
#define YON_API __declspec(dllimport)
#endif

//��������
//�÷�:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

#include <stdio.h>

#endif