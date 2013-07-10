#if defined(DEBUG_MEMORY)

#include "memdbg/commutex.h"

CCommonMutex::CCommonMutex()
{
#if defined WIN32
    InitializeCriticalSection( &m_Semaphore );
#elif defined LINUX
	sem_init( &m_Semaphore, 0, 1 );
#endif
	m_nCount = 0 ;	 
	m_nPid = 0 ;
}


CCommonMutex::~CCommonMutex()
{
#if defined WIN32
    DeleteCriticalSection( &m_Semaphore );
#elif defined LINUX
	sem_destroy( &m_Semaphore );
#endif

}
	
void CCommonMutex::Lock()
{
#if defined WIN32
	if(m_nPid != GetCurrentProcessId())
	{
		EnterCriticalSection( &m_Semaphore );
		m_nPid = GetCurrentProcessId();
	}	
#elif defined LINUX
	if(m_nPid != getpid())
	{
		sem_wait( &m_Semaphore );
		m_nPid = getpid();
	}	
#endif

	m_nCount++;
}

bool CCommonMutex::tryLock()
{
#if defined WIN32
	if(m_nPid != GetCurrentProcessId())
	{
		if(TryEnterCriticalSection( &m_Semaphore ) != 0) // _WIN32_WINNT >= 0x400
		{
			m_nPid = GetCurrentProcessId();
			m_nCount++;
			return TRUE ;
		}	
		else
		{
			return FALSE ;	
		}
	}	
	else
	{
		m_nCount++;
		return TRUE ;
	}
#elif defined LINUX
	if(m_nPid != getpid())
	{
		if(sem_trywait( &m_Semaphore )==0)
		{
			m_nPid = getpid();
			m_nCount++;
			return TRUE ;
		}	
		else
		{
			return FALSE ;	
		}
	}	
	else
	{
		m_nCount++;
		return TRUE ;
	}
#endif
}

void CCommonMutex::Unlock()
{
#if defined WIN32
    if(m_nPid == GetCurrentProcessId())
#elif defined LINUX
	if(m_nPid == getpid())
#endif
	{
		if(m_nCount == 1)
		{
			m_nPid = 0 ;				
		}	
		if(m_nCount > 0)
		{
			m_nCount--;
		}			
		if(m_nCount == 0)
#if defined WIN32
            LeaveCriticalSection( &m_Semaphore );
#elif defined LINUX
			sem_post( &m_Semaphore );
#endif
	}	
}

#endif // DEBUG_MEMORY