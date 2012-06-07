#include "CTimerAndroid.h"

#ifdef YON_COMPILE_WITH_ANDROID
#include <time.h>
#include <sys/time.h>

namespace yon{
	namespace platform{
		
		CTimerAndroid::CTimerAndroid()
			:m_uVirtualTime(0),
			m_bSetup(false),m_uLastStaticTime(0),m_uStartStaticTime(0),
			m_uStaticTime(0),m_fVirtualTimerSpeed(1.0f){}

		CTimerAndroid::~CTimerAndroid(){}

		u32 CTimerAndroid::getStaticTime(){
			timeval tv;
			gettimeofday(&tv, 0);
			return (u32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
		}

		void CTimerAndroid::start(){
			m_bSetup=true;
			m_uStaticTime=m_uStartStaticTime=m_uLastStaticTime=getStaticTime();
		}
		void CTimerAndroid::pause(){
			m_bSetup=false;
		}
		void CTimerAndroid::resume(){
			m_bSetup=true;
		}
		void CTimerAndroid::setSpeed(f32 speed){
			m_fVirtualTimerSpeed=speed;
		}
		u32 CTimerAndroid::getRealTime(){
			//return m_uStaticTime-m_uStartStaticTime;
			//如果还没start()，则返回0
			if(m_uStartStaticTime==0)
				return 0;
			return getStaticTime()-m_uStartStaticTime;
		}
		void CTimerAndroid::tick(){
			m_uLastStaticTime=m_uStaticTime;
			m_uStaticTime = getStaticTime();
			if(m_bSetup){
				m_uVirtualTime +=(u32)((m_uStaticTime - m_uLastStaticTime) * m_fVirtualTimerSpeed);
			}
		}

		
	}

	ITimer* createTimer(){
		return new platform::CTimerAndroid();
	}
}

#endif