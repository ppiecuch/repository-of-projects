#include "CTimerWin32.h"

#ifdef YON_COMPILE_WITH_WIN32

namespace yon{
namespace platform{

	CTimerWin32::CTimerWin32()
		:m_bHighAccurateSupport(false),m_bMultiCore(false),m_uVirtualTime(0),
		m_bSetup(false),m_uLastStaticTime(0),m_uStartStaticTime(0),
		m_uStaticTime(0),
		m_fVirtualTimerSpeed(1.0f){
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

		m_bMultiCore = (sysinfo.dwNumberOfProcessors > 1);
		m_bHighAccurateSupport = QueryPerformanceFrequency(&m_performanceFreq);
	}
	CTimerWin32::~CTimerWin32(){}
	void CTimerWin32::start(){
		m_bSetup=true;
		m_uStaticTime=m_uStartStaticTime=m_uLastStaticTime=getStaticTime();
	}
	void CTimerWin32::pause(){
		m_bSetup=false;
	}
	void CTimerWin32::resume(){
		m_bSetup=true;
	}	
	void CTimerWin32::setSpeed(f32 speed){
		m_fVirtualTimerSpeed=speed;
	}
	u32 CTimerWin32::getStaticTime(){
		if(m_bHighAccurateSupport){
			// Avoid potential timing inaccuracies across multiple cores by
			// temporarily setting the affinity of this process to one core.
			static DWORD_PTR affinityMask;
			if(m_bMultiCore)
				affinityMask = SetThreadAffinityMask(GetCurrentThread(), 1);
			static LARGE_INTEGER nTime;
			static BOOL queriedOK;
			queriedOK = QueryPerformanceCounter(&nTime);
			// Restore the true affinity.
			if(m_bMultiCore)
				SetThreadAffinityMask(GetCurrentThread(), affinityMask);
			if(queriedOK)
				return u32((nTime.QuadPart) * 1000 / m_performanceFreq.QuadPart);
		}
		return GetTickCount();
	}
	u32 CTimerWin32::getTime() const{
		return m_uVirtualTime;
	}
	u32 CTimerWin32::getRealTime(){
		//return m_uStaticTime-m_uStartStaticTime;
		//如果还没start()，则返回0
		if(m_uStartStaticTime==0)
			return 0;
		return getStaticTime()-m_uStartStaticTime;
	}
	void CTimerWin32::tick(){
		m_uLastStaticTime=m_uStaticTime;
		m_uStaticTime = getStaticTime();
		if(m_bSetup){
			m_uVirtualTime +=(u32)((m_uStaticTime - m_uLastStaticTime) * m_fVirtualTimerSpeed);
		}
	}
}

	ITimer* createTimer(){
		return new platform::CTimerWin32();
	}
}
#endif