#include "XCMutex.h"
#include "ILogger.h"

CXCMutex::CXCMutex(void)
{
#if defined(_PS3)
	sys_lwmutex_attribute_t lwmutex_attr;
	sys_lwmutex_attribute_initialize(lwmutex_attr);
	sys_lwmutex_create(&m_mutex, &lwmutex_attr);
#elif _WIN32
    m_mutex = CreateMutex(NULL,FALSE,NULL);
#elif IPHONEOS || _LINUX || defined (TARGET_MACOSX) || defined(ANDROID_NDK)
	MUTEX_SETUP(m_mutex);
#endif
}

CXCMutex::~CXCMutex(void)
{
#if defined(_PS3)
	sys_lwmutex_destroy(&m_mutex);
#elif _WIN32
	CloseHandle(m_mutex);
#elif IPHONEOS || _LINUX || defined (TARGET_MACOSX) || defined(ANDROID_NDK)
	switch( MUTEX_CLEANUP(m_mutex) )
	{
		case 0:		// success
			break;
			
		default:
			break;			
	}
#endif
}

#ifdef _DEBUG
void CXCMutex::Lock(const char* file,int line)
{
	//YON_DEBUG("Lock:%s,%d\r\n",file,line);
#else
void CXCMutex::Lock()
{
#endif
#if defined(_PS3)
	sys_lwmutex_lock(&m_mutex, 0);
#elif _WIN32
	WaitForSingleObject(m_mutex,INFINITE);
#elif IPHONEOS || _LINUX || defined (TARGET_MACOSX) || defined(ANDROID_NDK)
	switch( MUTEX_LOCK(m_mutex) )
	{
		case 0:			// Mutex succesfully locked
			break;
			
		case EBUSY:		//  Mutex is already locked by another thread
			break;
	}
#endif
}

#ifdef _DEBUG
void CXCMutex::Unlock(const char* file,int line)
{
	//YON_DEBUG("Unlock:%s,%d\r\n",file,line);
#else
void CXCMutex::Unlock()
{
#endif
#if defined(_PS3)
	sys_lwmutex_unlock(&m_mutex);
#elif _WIN32
	ReleaseMutex(m_mutex);
#elif IPHONEOS || _LINUX || defined (TARGET_MACOSX) || defined(ANDROID_NDK)
	switch( MUTEX_UNLOCK(m_mutex) )
	{
		case 0:		// Mutex succesfully unlocked
			break;
			
		case EPERM:	// Mutex is not owned by this thread
			break;	
	}
#endif
}
