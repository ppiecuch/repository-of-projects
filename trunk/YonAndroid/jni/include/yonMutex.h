#ifndef _YON_CORE_YONMUTEX_H_
#define _YON_CORE_YONMUTEX_H_

#include "IReferencable.h"
#include "ILogger.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <Windows.h>
#elif defined(YON_COMPILE_WITH_PS3)
#include <sys/synchronization.h>
#else
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#endif

namespace yon{
namespace core{

	class mutex{

#ifdef YON_COMPILE_WITH_WIN32
	typedef HANDLE MutexType;
#elif defined(YON_COMPILE_WITH_PS3)
	typedef sys_lwmutex_t MutexType;
#else
	typedef pthread_mutex_t MutexType;
#endif

	public:
		mutex(){
#ifdef YON_COMPILE_WITH_WIN32
			m_mutex = CreateMutex(NULL,FALSE,NULL);
#elif defined(YON_COMPILE_WITH_PS3)
			sys_lwmutex_attribute_t lwmutex_attr;
			sys_lwmutex_attribute_initialize(lwmutex_attr);
			sys_lwmutex_create(&m_mutex, &lwmutex_attr);
#else
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&(x), &attr);
#endif
		}
		~mutex(){
#ifdef YON_COMPILE_WITH_WIN32
			if(m_mutex&&!CloseHandle(m_mutex))
				YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("CloseHandle failed for %d",GetLastError()).c_str());
#elif defined(YON_COMPILE_WITH_PS3)
			sys_lwmutex_destroy(&m_mutex);
#else
			if(pthread_mutex_destroy(&m_mutex))
				YON_WARN(YON_LOG_WARN_FORMAT,"Release mutex failed!");
#endif
		}
		void wait(){
#ifdef YON_COMPILE_WITH_WIN32
			WaitForSingleObject(m_mutex,INFINITE);
#elif defined(YON_COMPILE_WITH_PS3)
			sys_lwmutex_lock(&m_mutex, 0);
#else
			if(pthread_mutex_lock(&m_mutex)==EBUSY)	//  Mutex is already locked by another thread
				YON_WARN(YON_LOG_WARN_FORMAT,"Mutex is already locked by another thread!");
#endif
		}
		void notify(){
#ifdef YON_COMPILE_WITH_WIN32
			ReleaseMutex(m_mutex);
#elif defined(YON_COMPILE_WITH_PS3)
			sys_lwmutex_unlock(&m_mutex);
#else
			if(pthread_mutex_unlock(&m_mutex)==EPERM)	// Mutex is not owned by this thread
				YON_WARN(YON_LOG_WARN_FORMAT,"Mutex is not owned by this thread!");
#endif
		}

	private:
		MutexType m_mutex;
	};

	class ScopeMutex{
	private:
		ScopeMutex(const ScopeMutex&);
		ScopeMutex& operator=(const ScopeMutex&);
		mutex& m_mutex;
	public:
		ScopeMutex(mutex& m):m_mutex(m){
			m_mutex.wait();
		}
		~ScopeMutex(){m_mutex.notify();}
	};
}
}
#endif