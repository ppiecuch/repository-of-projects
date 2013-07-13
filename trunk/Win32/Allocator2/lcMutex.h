#ifndef _LUCID_MUTEX_H_
#define _LUCID_MUTEX_H_

#include "lcDefine.h"

#ifdef LC_CMPL_MSVC
#include <Windows.h>
#elif defined(LC_CMPL_GCC)
#include <errno.h>
#include <pthread.h>
#endif


namespace lc{

class mutex{
private:
#ifdef LC_CMPL_MSVC
	typedef HANDLE MutexType;
#elif defined(LC_CMPL_GCC)
	typedef pthread_mutex_t MutexType;
#endif
	MutexType m_mutex;
public:
	mutex();
	~mutex();
	void lock();
	void unlock();
};

}
#endif