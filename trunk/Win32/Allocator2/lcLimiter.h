#ifndef _LUCID_LIMITER_H_
#define _LUCID_LIMITER_H_

#include "lcMutex.h"
#include "lcTimer.h"

namespace lc{

template<typename T,bool safe=false,size_t time_slot=0>
class limiter{
public:
	typedef void (*pCallback)(T value);
protected:
	struct Slot{
		u32 Time;
		T Value;
	};
	void update();
	
	mutex m_mutex;
	T m_accumulator;
	T m_limit;
	pCallback m_pCallback;
public:

	~Limiter(){}

	bool add(T value){
		if(safe)
			m_mutex.lock();

		if(m_accumulator+value>m_limit)
			return false;

		m_accumulator+=value;
		update();

		if(safe)
			m_mutex.unlock();

		return true;
	}

	void setExceedCallback(pCallback pFunc){
		m_pCallback=pFunc;
	}

	void setLimit(T value){
		m_limit=value;
	}
};
}
#endif