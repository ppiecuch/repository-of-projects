#include "lcTimer.h"

#ifndef LC_CMPL_MSVC
#include <time.h>
#include <sys/time.h>
#endif

namespace lc{

	template<> timer* core::Singleton<timer>::s_pSingleton = NULL;

#ifdef LC_CMPL_MSVC
	timer::timer():m_bHighAccurateSupport(false),m_bMultiCore(false){
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

		m_bMultiCore = (sysinfo.dwNumberOfProcessors > 1);
		m_bHighAccurateSupport = QueryPerformanceFrequency(&m_performanceFreq);
	}
#else
	timer::timer(){}
#endif
	
	u32 timer::getTime() const{
#ifdef LC_CMPL_MSVC
		//TODO �Ƿ���Ҫmutex.lock?
		if(m_bHighAccurateSupport){
			// Avoid potential timing inaccuracies across multiple cores by
			// temporarily setting the affinity of this process to one core.
			DWORD_PTR affinityMask;
			if(m_bMultiCore)
				affinityMask = SetThreadAffinityMask(GetCurrentThread(), 1);
			LARGE_INTEGER nTime;
			BOOL queriedOK;
			queriedOK = QueryPerformanceCounter(&nTime);
			// Restore the true affinity.
			if(m_bMultiCore)
				SetThreadAffinityMask(GetCurrentThread(), affinityMask);
			if(queriedOK)
				return u32((nTime.QuadPart) * 1000 / m_performanceFreq.QuadPart);
		}
		//��Ҫ��������1���������£����Բ���GetTickCount()�������ú����ķ���ֵ��DWORD�ͣ���ʾ�Ժ���Ϊ��λ�ļ��������������ʱ������
		return GetTickCount();
#else
		timeval tv;
		gettimeofday(&tv, 0);
		return (u32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
	}
}