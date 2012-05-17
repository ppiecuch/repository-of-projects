#ifndef _LOG_H_
#define _LOG_H_
#include <android/log.h>
//在标准C里，你不能省略可变参数，但是你却可以给它传递一个空的参数。为了解决这个问题，CPP使用一个特殊的‘##’操作。
#define LOGD(title,fmt,...) __android_log_print(ANDROID_LOG_DEBUG,title,fmt,##__VA_ARGS__)
#define LOGI(title,fmt,...) __android_log_print(ANDROID_LOG_INFO,title,fmt,##__VA_ARGS__)
#define LOGW(title,fmt,...) __android_log_print(ANDROID_LOG_WARN,title,fmt,##__VA_ARGS__)
#define LOGE(title,fmt,...) __android_log_print(ANDROID_LOG_ERROR,title,fmt,##__VA_ARGS__)
#endif
