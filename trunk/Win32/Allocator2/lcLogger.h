#ifndef _LUCID_LOGGER_H_
#define _LUCID_LOGGER_H_

#include "lcTypes.h"

#if defined(LC_OS_WINNT)||defined(LC_OS_WINRT)
#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <tchar.h>

namespace lc{

	void trace(const lc::c8 * pszFormat, ...);

}

#define LC_DEBG(fmt,...) lc::trace(fmt,__VA_ARGS__)
#define LC_INFO(fmt,...) lc::trace(fmt,__VA_ARGS__)
#define LC_WARN(fmt,...) lc::trace(fmt,__VA_ARGS__)
#define LC_EROR(fmt,...) lc::trace(fmt,__VA_ARGS__)

#elif defined(LC_OS_ANDROID)

#include <android/log.h>
//在标准C里，你不能省略可变参数，但是你却可以给它传递一个空的参数。为了解决这个问题，CPP使用一个特殊的‘##’操作。
#define LC_DEBG(fmt,...) __android_log_print(ANDROID_LOG_DEBUG,LC_FILE,fmt,##__VA_ARGS__)
#define LC_INFO(fmt,...) __android_log_print(ANDROID_LOG_INFO,LC_FILE,fmt,##__VA_ARGS__)
#define LC_WARN(fmt,...) __android_log_print(ANDROID_LOG_WARN,LC_FILE,fmt,##__VA_ARGS__)
#define LC_EROR(fmt,...) __android_log_print(ANDROID_LOG_ERROR,LC_FILE,fmt,##__VA_ARGS__)

#else

#define LC_DEBG(fmt,...) 
#define LC_INFO(fmt,...) 
#define LC_WARN(fmt,...) 
#define LC_EROR(fmt,...) 

#endif
#endif
