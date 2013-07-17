#include "lcThrottle.h"
#include "lcTimer.h"
#include "lcUtil.h"

namespace lc{

	//refer to:http://www.oschina.net/code/explore/android-4.0.1/media/libstagefright/ThrottledSource.cpp
	//refer to:http://www.oschina.net/code/explore/android-4.0.1/media/libstagefright/include/ThrottledSource.h

	Throttle::Throttle(u32 threshold):m_uThreshold(threshold),m_uTransfered(0),m_uStartTime(0){
	}

	void Throttle::setThreshold(u32 threshold){
		m_mutex.lock();
		m_uThreshold=threshold;
		m_uStartTime=0;
		m_uTransfered=0;
		m_mutex.unlock();
	}

	void Throttle::onTransfer(u32 size){
		u32 time=timer::getInstance().getTime();
		
		m_mutex.lock();
		m_uTransfered+=size;

		if(m_uStartTime==0)
			m_uStartTime=time;
		else
		{
			u32 duration=m_uTransfered*1000/m_uThreshold;
			u32 expected=m_uStartTime+duration;
			if(expected>time)
			{
				lc::sleep(expected-time);
			}
		}
		m_mutex.unlock();
	}
}