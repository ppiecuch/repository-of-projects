#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_ENGINE_NAME "YonEngine"

#define YON_VERSION_MAJOR 0 //主版本号
#define YON_VERSION_MINOR 1	//子版本号
#define YON_VERSION_PATCH 0 //修订版本号

//编译平台(Win32/Android/MacOSX)
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

//视频模式(OpenGLES1/OpenGLES2)
#define  YON_VIDEO_MODE_OGLES1
//#define  YON_VIDEO_MODE_OGLES2

//音频模式(OpenAL)
#define YON_AUDIO_MODE_OAL

//DLL导入导出
#ifdef YON_EXPORTS
#define YON_API __declspec(dllexport)
//#define YON_API extern "C"
#else
#define YON_API __declspec(dllimport)
//#define YON_API extern "C"
#endif

//日志格式
#ifdef YON_COMPILE_WITH_WIN32
#define YON_LOG_SUCCEED_FORMAT "%-64s【√】\r\n"
#define YON_LOG_FAILED_FORMAT "%-64s【╳】\r\n"
#define YON_LOG_WARN_FORMAT "%-64s【!】\r\n"
#else
#define YON_LOG_SUCCEED_FORMAT "%-64s\u3010\u221A\u3011\r\n"
#define YON_LOG_FAILED_FORMAT "%-64s\u3010\u2573\u3011\r\n"
#define YON_LOG_WARN_FORMAT "%-64s\u3010!\u3011\r\n"
#endif

//路径使用宽字节
//#define YON_WCHAR_FILESYSTEM

//隐藏控制台
//#define YON_HIDE_CONSOLE

//过期声明
//用法:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

//内存泄漏检测
#if defined(YON_COMPILE_WITH_WIN32)&&defined(_DEBUG)&&!defined(_AFX)||defined(ENFORCE_CHECK_MEMORY)
#include <crtdbg.h>
#define YON_CHECK_MEMORY
#define YON_ORIGIN_NEW new
#define YON_DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

//材质（目前只支持一层）
#define YON_MATERIAL_MAX_TEXTURES 2

//触控
#define YON_TOUCH_MAX_INPUTS 8

//宏修正
#ifdef YON_COMPILE_WITH_WIN32
//#elif defined(YON_COMPILE_WITH_ANDROID)
#else
//Unix和Linux系统不支持unicode.没有任何unicode版库函数.
#undef YON_WCHAR_FILESYSTEM
#undef YON_DEPRECATED
#undef YON_API
#define YON_DEPRECATED
#define YON_API
#endif

#include <stdio.h>

#endif