#ifndef _YON_CONFIG_H_
#define _YON_CONFIG_H_

#define YON_VERSION_MAJOR 0 //主版本号
#define YON_VERSION_MINOR 1	//子版本号

//编译平台(Win32/Android/MacOSX)
#define YON_COMPILE_WITH_WIN32
//#define YON_COMPILE_WITH_ANDROID
//#define YON_COMPILE_WITH_MACOSX

//视频模式(OpenGLES1/OpenGLES2)
#define  YON_VIDEO_MODE_OGLES1
//#define  YON_VIDEO_MODE_OGLES2

//DLL导入导出
#ifdef YON01_EXPORTS
#define YON_API __declspec(dllexport)
#else
#define YON_API __declspec(dllimport)
#endif

//隐藏控制台
//#define YON_HIDE_CONSOLE

//过期声明
//用法:
//YON_DEPRECATED void fun(...){...}
//class YON_DEPRECATED object{...}
#define YON_DEPRECATED __declspec(deprecated)

#include <stdio.h>

#endif