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
#ifdef YON_EXPORTS
#define YON_API __declspec(dllexport)
#else
#define YON_API __declspec(dllimport)
#endif

//��־��ʽ
#define YON_LOG_SUCCEED_FORMAT "%-64s���̡�\n"
#define YON_LOG_FAILED_FORMAT "%-64s���w��\n"

//·��ʹ�ÿ��ֽ�
#define YON_WCHAR_FILESYSTEM

//���ؿ���̨
//#define YON_HIDE_CONSOLE

//��������
#define YON_DEPRECATED __declspec(deprecated)

#include <stdio.h>

#endif