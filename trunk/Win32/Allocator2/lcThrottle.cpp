#include "lcThrottle.h"

namespace lc{

	//refer to:http://www.oschina.net/code/explore/android-4.0.1/media/libstagefright/ThrottledSource.cpp
	//refer to:http://www.oschina.net/code/explore/android-4.0.1/media/libstagefright/include/ThrottledSource.h

	Throttle::Throttle(u32 threshold):m_uThreshold(threshold){
	}

	void Throttle::setThreshold(u32 threshold){
		m_uThreshold=threshold;
	}

	void Throttle::onTransfer(u32 size, u32 time){

	}
}