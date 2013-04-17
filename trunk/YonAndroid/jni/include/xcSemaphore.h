#ifndef _XC_CORE_XCSEMAPHORE_H_
#define _XC_CORE_XCSEMAPHORE_H_

#include "IReferencable.h"
#include "xcString.h"
#include "ILogger.h"

#ifdef XC_COMPILE_WITH_WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#endif

namespace xc{
namespace core{

	/**
	 * �ź������ǿ��������������������ؼ�����Σ���Щ�ؼ�����β��ܲ������á�
	 * �ڽ���һ���ؼ������֮ǰ���̱߳����ȡһ���ź�����
	 * ����ؼ��������û���κ��̣߳���ô�̻߳���������ÿ�ͼ�е��Ǹ����֡�
	 * һ���ùؼ����������ˣ���ô���̱߳����ͷ��ź��������������ùؼ�����ε��̱߳���ȴ�ֱ����һ���߳��ͷ��ź�����
	 */
	class semaphore{

#ifdef XC_COMPILE_WITH_WIN32
	typedef HANDLE sem_t;
#endif

	public:
		semaphore(s32 maxNum=1){
#ifdef XC_COMPILE_WITH_WIN32
			core::stringc name("%08X",this);
			m_semaphore=CreateSemaphoreA(NULL,maxNum,maxNum,name.c_str());
			if(m_semaphore==NULL)
				XC_ERROR(XC_LOG_FAILED_FORMAT,core::stringc("CreateSemaphore failed for %d",GetLastError()).c_str());
#else
			sem_init( &m_semaphore, 0, maxNum );
#endif
		}
		~semaphore(){
#ifdef XC_COMPILE_WITH_WIN32
			if(m_semaphore&&!CloseHandle(m_semaphore))
				XC_ERROR(XC_LOG_FAILED_FORMAT,core::stringc("CloseHandle failed for %d",GetLastError()).c_str());
#else
			sem_destroy( &m_semaphore );
#endif
		}
		void wait(){
#ifdef XC_COMPILE_WITH_WIN32
			XC_DEBUG_BREAK_IF(m_semaphore==NULL);
			WaitForSingleObject(m_semaphore,INFINITE);
#else
			sem_wait( &m_semaphore );
#endif
		}
		void notify(){
#ifdef XC_COMPILE_WITH_WIN32
			XC_DEBUG_BREAK_IF(m_semaphore==NULL);
			ReleaseSemaphore(m_semaphore,1,NULL);
#else
			sem_post( &m_semaphore );
#endif
		}
	private:
		sem_t m_semaphore;
	};
}
}
#endif