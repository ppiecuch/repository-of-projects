/****************************************\
*										*
*  Desc		: 跨平台的互斥量封装类 *
			  网络模块中使用
*  Author	: Guo Huafeng				*
*  Date     : 2012-10					*
\****************************************/

#pragma once

#if defined(_PS3)
#include <sys/synchronization.h>
typedef sys_lwmutex_t mutexType;
#elif _WIN32
#include <windows.h>
typedef HANDLE mutexType;
#elif defined(IPHONEOS) || defined (_LINUX) || defined(TARGET_MACOSX) || defined(FREEBOX) || defined(ANDROID_NDK)
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
typedef pthread_mutex_t mutexType;
#else // Dummy impl
#error select a platform
typedef int mutexType;
#endif

#if defined(IPHONEOS) || defined(_LINUX) || defined (TARGET_MACOSX) || defined(ANDROID_NDK)
#define	MUTEX_SETUP(x)		\
	do {	\
		pthread_mutexattr_t attr;		\
		pthread_mutexattr_init(&attr);	\
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
		pthread_mutex_init(&(x), &attr);	\
	} while(0)

#define	MUTEX_CLEANUP(x)	pthread_mutex_destroy(&(x))
#define	MUTEX_LOCK(x)		pthread_mutex_lock(&(x))
#define	MUTEX_UNLOCK(x)		pthread_mutex_unlock(&(x))
#define	MUTEX_TRYLOCK(x)	pthread_mutex_trylock(&(x))
#endif

#include "yonString.h"
using namespace yon;

// 
// 互斥量
//
class CXCMutex
{
public:
	CXCMutex(void);
	~CXCMutex(void);

public:

#ifdef _DEBUG
	void Lock(const char* file,int line);
	void Unlock(const char* file,int line);
#else
	void Lock();
	void Unlock();
#endif
	

private:
	mutexType m_mutex;	//平台相关的互斥量
};

//
// 作用域内互斥量
// 通常用在函数、方法内，自动锁定和解锁
//
class CScopedMutexLock
{
#ifdef _DEBUG
	core::stringc m_file;
	s32 m_line;
public:
	//构造函数锁定互斥量
	CScopedMutexLock(CXCMutex& mutex,const char* file,int line):m_file(file),m_line(line),m_mutex(mutex) 
	{ 
		m_mutex.Lock(file,line); 
	}
	//析构函数解锁互斥量
	~CScopedMutexLock() { m_mutex.Unlock(m_file.c_str(),m_line); }
#else
public:
	//构造函数锁定互斥量
	CScopedMutexLock(CXCMutex& mutex) 
		: m_mutex(mutex) 
	{ 
		m_mutex.Lock(); 
	}
	//析构函数解锁互斥量
	~CScopedMutexLock() { m_mutex.Unlock(); }
#endif

	
private:
	CXCMutex& m_mutex;	//封装的互斥量
};
