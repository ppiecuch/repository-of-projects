#if defined (DEBUG_MEMORY)

#ifndef CCommonMutex_h
#define CCommonMutex_h 1

#include "memdbg/comheader.h"

class CCommonMutex
{
#if defined WIN32
    typedef CRITICAL_SECTION sem_t;
#endif
protected:
	 sem_t m_Semaphore;
private:
	UINT m_nCount ;	 
	int m_nPid ;
public:
	CCommonMutex();
	virtual ~CCommonMutex();
	void Lock();
	bool tryLock();
	void Unlock();
};

#endif

#endif // DEBUG_MEMORY
