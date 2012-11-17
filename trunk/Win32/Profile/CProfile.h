#ifndef _YON_DEBUG_CPROFILE_H_
#define _YON_DEBUG_CPROFILE_H_

#include "IProfile.h"

#include <time.h>
#include <sys/timeb.h>

namespace yon{
namespace debug{

	class CProfile : public IProfile{
	protected:
		bool m_bNeedUpdate;
		u64 m_uAllCallConsume;
		SProfileReport m_report;

#ifdef YON_COMPILE_WITH_WIN32
		struct Between{
			_timeb StartTime;
			_timeb EndTime;

			u64 StartCycle;
			u64 EndCycle;
		};
		inline void updateTime(_timeb& t){
			_ftime64_s(&t);
		}
#else
		struct Between{
			timeb StartTime;
			timeb EndTime;

			u64 StartCycle;
			u64 EndCycle;
		};
		inline void updateTime(timeb& t){
			ftime(&t);
		}
#endif
		Between m_between;
		u64 getCycleDiff(){
			return m_between.EndCycle-m_between.StartCycle;
		}

		u64 getTimeDiff(){
			return (m_between.EndTime.time-m_between.StartTime.time)*1000+m_between.EndTime.millitm-m_between.StartTime.millitm;
		}

		bool m_bCalling;
		void reset();
		void update();
		void reset(SProfileReport::SAPIReport* report);
		void updateCallStart(SProfileReport::SAPIReport* report);
		void updateCallEnd(SProfileReport::SAPIReport* report);
		inline unsigned __int64 getNanoSecond()   
		{
			__asm _emit 0x0F   
			__asm _emit 0x31   
		}
		
		typedef core::map<u32,SProfileReport::SAPIReport*> APIMap;
	public:
		CProfile();
		~CProfile();
		virtual void registerFrame();
		virtual void startCall(u32 id,const char* name);
		virtual void endCall(u32 id);
		virtual SProfileReport* getReport(){
			update();
			return &m_report;
		}
		virtual void report();
	};
}
}

#endif