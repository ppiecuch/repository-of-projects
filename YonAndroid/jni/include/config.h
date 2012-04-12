#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_ENGINE_NAME "YonEngine"

#define YON_VERSION_MAJOR 0 //主版本号
#define YON_VERSION_MINOR 1	//子版本号
#define YON_VERSION_REVISION 0//修订版本号

//编译平台(Win32/Android/MacOSX)
#ifdef WIN32
#define YON_COMPILE_WITH_WIN32
#elif defined(unix)
#define YON_COMPILE_WITH_ANDROID
#else
#define YON_COMPILE_WITH_MACOSX
#endif // WIN32

//视频模式(OpenGLES1/OpenGLES2)
#define  YON_VIDEO_MODE_OGLES1
//#define  YON_VIDEO_MODE_OGLES2

//DLL导入导出
#ifdef YON_EXPORTS
#define YON_API __declspec(dllexport)
#else
#define YON_API __declspec(dllimport)
#endif

//日志格式
#define YON_LOG_SUCCEED_FORMAT "%-64s【√】\n"
#define YON_LOG_FAILED_FORMAT "%-64s【╳】\n"
#define YON_LOG_WARN_FORMAT "%-64s【!】\n"

//路径使用宽字节
//#define YON_WCHAR_FILESYSTEM

//隐藏控制台
//#define YON_HIDE_CONSOLE

//过期声明
//用法:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

//材质（目前只支持一层）
#define YON_MATERIAL_MAX_TEXTURES 2

//宏修正
#ifdef YON_COMPILE_WITH_WIN32
#elif defined(YON_COMPILE_WITH_ANDROID)
//Unix和Linux系统不支持unicode.没有任何unicode版库函数.
#undef YON_WCHAR_FILESYSTEM
#undef YON_DEPRECATED
#undef YON_API
#define YON_DEPRECATED
#define YON_API
#endif

#include <stdio.h>

#endif