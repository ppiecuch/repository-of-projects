#ifndef _LUCID_LIMITER_H_
#define _LUCID_LIMITER_H_

#include "lcMutex.h"
#include "lcTimer.h"
#include "lcList.h"
#include "lcLogger.h"

namespace lc{

/*
template<typename T,size_t time_slot=0,bool safe=false>
class limiter{
public:
	typedef void (*pCallback)(T value);
protected:
	struct Slot{
		u32 Time;
		T Value;
	};
	typedef core::list<Slot> SlotList;
	void update(T value){
		if(time_slot==0)
		{
			if(m_pCallback&&m_accumulator>m_limit)
				m_pCallback(m_limit-m_accumulator);
		}
		else
		{
			Slot s;
			s.Time=timer::getInstance().getTime();
			s.Value=value;
			m_slots.push_back(s);

			u32 min=s.Time-time_slot;

			u32 i;
			for(i=0;i<m_slots.size();++i)
			{
				SlotList::Iterator it=m_slots.begin();
				if(it->Time>min)
					break;
				else
					m_slots.erase(it);
			}
			LC_DEBG("erase:%u\r\n",i);
			SlotList::Iterator it=m_slots.getLast();
			T totalValue=it->Value;
			u32 maxTime=it->Time;
			u32 minTime=it->Time;
			for(;;--it)
			{
				totalValue+=it->Value;
				minTime=it->Time;
				if(it==m_slots.begin())
					break;
			}

			T total=maxTime==minTime?totalValue:totalValue/(maxTime-minTime);

			if(m_pCallback&&total!=m_limit)
				m_pCallback(total-m_limit);
		}
	}
	
	SlotList m_slots;
	mutex m_mutex;
	T m_accumulator;
	T m_limit;
	pCallback m_pCallback;
public:

	limiter():m_pCallback(NULL){}

	~limiter(){
		m_slots.clear();
	}

	void add(T value){
		if(safe)
			m_mutex.lock();

		m_accumulator+=value;
		update(value);

		if(safe)
			m_mutex.unlock();
	}

	void setExceedCallback(pCallback pFunc){
		m_pCallback=pFunc;
	}

	void setLimit(T value){
		m_limit=value;
	}
};
*/

class FPSLimiter{
	u32 m_uFPS;
	f32 m_fRealFPS;
	u32 m_uTPF;
	u32 m_uStart,m_uEnd,m_uLast;
	u32 m_uFrameCount;
	bool m_bSlotFPS;
public:
	FPSLimiter(bool slot=false);
	void setFPS(u32 value);
	void tick();
	f32 getRealFPS() const;
};
}
#endif