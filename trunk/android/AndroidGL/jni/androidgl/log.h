#ifndef _LOG_H_
#define _LOG_H_
#include <android/log.h>
#define LOGD(title,fmt,...) __android_log_print(ANDROID_LOG_DEBUG,title,fmt,__VA_ARGS__)
#define LOGI(title,fmt,...) __android_log_print(ANDROID_LOG_INFO,title,fmt,__VA_ARGS__)
#define LOGW(title,fmt,...) __android_log_print(ANDROID_LOG_WARN,title,fmt,__VA_ARGS__)
#define LOGE(title,fmt,...) __android_log_print(ANDROID_LOG_ERROR,title,fmt,__VA_ARGS__)
#endif
