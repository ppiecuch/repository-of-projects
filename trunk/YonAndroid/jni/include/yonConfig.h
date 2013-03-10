#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_ENGINE_NAME "YonEngine"

#define YON_VERSION_MAJOR 0 //���汾��
#define YON_VERSION_MINOR 1	//�Ӱ汾��
#define YON_VERSION_PATCH 0 //�޶��汾��

//����ƽ̨(Win32/Android/MacOSX)
#ifdef WIN32
#define YON_COMPILE_WITH_WIN32
#elif defined(ANDROID_NDK)
#define YON_COMPILE_WITH_ANDROID
#ifndef __CYGWIN__
#define __CYGWIN__
#endif
#elif defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__))
#if defined(__ppc__)
#elif defined(__ppc64__)
#elif defined(__i386__)
#elif defined(__x86_64__)
#elif defined(__arm__)
#else
#error unrecognized GNU C compiler
#endif
#define YON_COMPILE_WITH_MACOSX
#else
#error unrecognized compile platform
#endif // WIN32

//CPUͨ�üĴ���λ��
#if defined __x86_64__ || defined _M_X64 || defined __powerpc64__ || defined __alpha__ || defined __ia64__ || defined __s390__ || defined __s390x__
#define YON_ARCH_BITMODE_64
#else
#define YON_ARCH_BITMODE_32
#endif

//��Ƶģʽ(OpenGLES1/OpenGLES2)
#define  YON_VIDEO_MODE_OGLES1
//#define  YON_VIDEO_MODE_OGLES2

//��Ƶģʽ(OpenAL)
//#define YON_AUDIO_MODE_OAL
#define YON_AUDIO_MODE_FMOD

//DLL���뵼��
#ifdef YON_EXPORTS
#define YON_API __declspec(dllexport)
#define YON_API_TPL __declspec(dllexport)
//#define YON_API extern "C"
#else
#define YON_API __declspec(dllimport)
#define YON_API_TPL
//#define YON_API extern "C"
#endif

//��־��ʽ
#define YON_USE_LOGGER
#ifdef DISABLE_LOGGER
#undef YON_USE_LOGGER
#endif
#ifdef YON_COMPILE_WITH_WIN32
#define YON_LOG_SUCCEED_FORMAT "%-64s���̡�\r\n"
#define YON_LOG_FAILED_FORMAT "%-64s���w��\r\n"
#define YON_LOG_WARN_FORMAT "%-64s��!��\r\n"
#else
#define YON_LOG_SUCCEED_FORMAT "%-64s\u3010\u221A\u3011\r\n"
#define YON_LOG_FAILED_FORMAT "%-64s\u3010\u2573\u3011\r\n"
#define YON_LOG_WARN_FORMAT "%-64s\u3010!\u3011\r\n"
#endif

//Profile����
#define YON_USE_PROFILE
#ifdef DISABLE_PROFILE
#undef YON_USE_PROFILE
#endif

//·��ʹ�ÿ��ֽ�
//#define YON_WCHAR_FILESYSTEM

//���ؿ���̨
//#define YON_HIDE_CONSOLE

//��������
//�÷�:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

//�ڴ�й©���
#if defined(YON_COMPILE_WITH_WIN32)&&defined(_DEBUG)&&!defined(_AFX)||defined(ENFORCE_CHECK_MEMORY)
#include <crtdbg.h>
#define YON_CHECK_MEMORY
#define YON_ORIGIN_NEW new
#define YON_DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

//���ʣ�Ŀǰֻ֧��һ�㣩
#define YON_MATERIAL_MAX_TEXTURES 2
//ֻ����2D��Ϸ
#define YON_ONLY_2D_GAME

//����
#define YON_TOUCH_MAX_INPUTS 8

//������
#ifdef YON_COMPILE_WITH_WIN32
//#elif defined(YON_COMPILE_WITH_ANDROID)
#else
//Unix��Linuxϵͳ��֧��unicode.û���κ�unicode��⺯��.
#undef YON_WCHAR_FILESYSTEM
#undef YON_DEPRECATED
#undef YON_API
#define YON_DEPRECATED
#define YON_API
#endif

#include <stdio.h>
#endif