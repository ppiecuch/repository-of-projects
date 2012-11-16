#ifndef _YON_DEBUG_CPROFILE_H_
#define _YON_DEBUG_CPROFILE_H_

#include "IProfile.h"

namespace yon{
namespace debug{

	class CProfile : public IProfile{
	protected:
		bool m_bNeedUpdate;
		u64 m_uStartTime;
		u64 m_uEndTime;
		u64 m_uAllCallConsume;
		SProfileReport m_report;
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
		
		typedef core::map<void*,SProfileReport::SAPIReport*> APIMap;
	public:
		CProfile();
		~CProfile();
		virtual void registerFrame();
		virtual void startCall(void* fun,const char* name);
		virtual void endCall(void* fun);
		virtual SProfileReport* getReport(){
			update();
			return &m_report;
		}
		virtual void report();
	};
}
}

#endif