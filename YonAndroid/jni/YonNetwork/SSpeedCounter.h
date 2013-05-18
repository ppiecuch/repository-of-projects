#ifndef _SSPEEDCOUNTER_H_
#define _SSPEEDCOUNTER_H_

#include "yonUtil.h"
#include "yonString.h"
#include "yonList.h"
#include "ITimer.h"

using namespace yon;

struct SSpeedCounter{
private:

	struct SBlock{
		u32 Time;
		u64 Length;
	};

	typedef core::list<SBlock> BlockList;

	BlockList m_blocks;
	u32 m_uTimeSpan;
	ITimer* m_pTimer;

public:
	SSpeedCounter(ITimer* timer,u32 timeSpan):m_uTimeSpan(timeSpan),m_pTimer(timer){
	}

	~SSpeedCounter(){
		m_blocks.clear();
	}

	void increase(u64 len){
		SBlock block;
		block.Time=m_pTimer->getRealTime();
		block.Length=len;
		m_blocks.push_back(block);
	}

	core::stringc getSpeed(){
		u32 time=m_pTimer->getRealTime();
		u32 limit=time-m_uTimeSpan;
		u32 min=time;

		BlockList::Iterator it=m_blocks.begin();
		for(;it!=m_blocks.end();++it)
		{
			SBlock b=*it;
			if(b.Time<limit)
				m_blocks.erase(it);
			else if(b.Time<min)
				min=b.Time;
		}

		u64 len=0;
		it=m_blocks.begin();
		for(;it!=m_blocks.end();++it)
		{
			len+=(*it).Length;
		}

		f32 speed=len==0?0:len*1000/((f32)(time-min)+0.0001f);
		return core::yonFormatSize(speed);
	}
};
#endif