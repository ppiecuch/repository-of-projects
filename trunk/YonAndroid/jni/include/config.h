#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_ENGINE_NAME "YonEngine"

#define YON_VERSION_MAJOR 0 //���汾��
#define YON_VERSION_MINOR 1	//�Ӱ汾��
#define YON_VERSION_REVISION 0//�޶��汾��

//����ƽ̨(Win32/Android/MacOSX)
#ifdef WIN32
#define YON_COMPILE_WITH_WIN32
#elif defined(unix)
#define YON_COMPILE_WITH_ANDROID
#else
#define YON_COMPILE_WITH_MACOSX
#endif // WIN32

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
#define YON_LOG_WARN_FORMAT "%-64s��!��\n"

//·��ʹ�ÿ��ֽ�
//#define YON_WCHAR_FILESYSTEM

//���ؿ���̨
//#define YON_HIDE_CONSOLE

//��������
//�÷�:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

//���ʣ�Ŀǰֻ֧��һ�㣩
#define YON_MATERIAL_MAX_TEXTURES 2

//������
#ifdef YON_COMPILE_WITH_WIN32
#elif defined(YON_COMPILE_WITH_ANDROID)
//Unix��Linuxϵͳ��֧��unicode.û���κ�unicode��⺯��.
#undef YON_WCHAR_FILESYSTEM
#undef YON_DEPRECATED
#undef YON_API
#define YON_DEPRECATED
#define YON_API
#endif

#include <stdio.h>

#endif