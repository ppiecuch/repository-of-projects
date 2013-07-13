#include "lcMutex.h"

//How to create a single instance application in C
//refer to:http://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c

namespace lc{

	mutex::mutex():m_mutex(NULL){
#ifdef LC_CMPL_MSVC
		m_mutex = ::CreateMutex(NULL, FALSE, NULL);
#elif defined(LC_CMPL_GCC)
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		//TODO 真能跨进程
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_mutex, &attr);
#endif

	}
	mutex::~mutex(){
#ifdef LC_CMPL_MSVC
		if(m_mutex)
			CloseHandle(m_mutex);
#elif defined(LC_CMPL_GCC)
		if(m_mutex)
			pthread_mutex_destroy(&m_mutex);
#endif
	}

	void mutex::lock(){
#ifdef LC_CMPL_MSVC
		WaitForSingleObject(m_mutex,INFINITE);
#elif defined(LC_CMPL_GCC)
		if(pthread_mutex_lock(&m_mutex)==EBUSY)	//  Mutex is already locked by another thread
			YON_WARN(YON_LOG_WARN_FORMAT,"Mutex is already locked by another thread!");
#endif
	}
	void mutex::unlock(){
#ifdef LC_CMPL_MSVC
		ReleaseMutex(m_mutex);
#elif defined(LC_CMPL_GCC)
		if(pthread_mutex_unlock(&m_mutex)==EPERM)	// Mutex is not owned by this thread
			YON_WARN(YON_LOG_WARN_FORMAT,"Mutex is not owned by this thread!");
#endif
	}
}