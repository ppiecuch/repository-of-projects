#include "lcLimiter.h"
#include "lcTimer.h"
#include "lcUtil.h"
#include "lcLogger.h"

namespace lc{

	FPSLimiter::FPSLimiter(bool slot):m_uStart(0),m_uEnd(0),m_uFPS(0),m_uTPF(0),m_fRealFPS(60.f),m_uLast(0),m_uFrameCount(0),m_bSlotFPS(slot){
		setFPS(60);
	}

	void FPSLimiter::setFPS(u32 value){
		m_uFPS=value;
		m_uTPF=1000/value;
	}

	void FPSLimiter::tick(){
		m_uEnd=timer::getInstance().getTime();
		u32 diff=m_uEnd-m_uStart;
		if(m_uTPF>diff)
			lc::sleep(m_uTPF-diff);
		else
			LC_DEBG("Diff:%u exceed TPF:%u, FPSLimiter no sleep%8s\r\n",diff,m_uTPF,LC_SYMBOL_WARN);
		u32 curr=timer::getInstance().getTime();
		if(m_bSlotFPS)
		{
			if(m_uLast==0)
				m_uLast=m_uEnd;
			++m_uFrameCount;
			if(m_uEnd-m_uLast>1500)
			{
				m_fRealFPS=m_uFrameCount*1000.f/(m_uEnd-m_uLast);
				m_uLast=m_uEnd;
				m_uFrameCount=0;
			}
		}
		else
			m_fRealFPS=1000.f/(curr-m_uStart);
		m_uStart=curr;
	}

	f32 FPSLimiter::getRealFPS() const{
		return m_fRealFPS;
	}
}