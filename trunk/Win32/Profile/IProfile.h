#ifndef _YON_DEBUG_IPROFILE_H_
#define _YON_DEBUG_IPROFILE_H_

#include "IReferencable.h"
#include "yonString.h"
#include "yonMap.h"

namespace yon{
namespace debug{

	struct SProfileReport{
		struct SAPIReport{
			core::stringc Name;
			u32 CallCount;
			f32 CallCountAvg;
			u64 TimeConsume;
			u64 TimeConsumeMin;
			u64 TimeConsumeMax;
			u64 TimeConsumeAvg;
			f32 TimeConsumePct;
			u64 _calltime;
		};
		u32 FrameCount;
		u64 TimeDiff;
		core::map<void*,SAPIReport*> ApiInfos;
	};

	class IProfile : public core::IReferencable{
	public:
		virtual void registerFrame() = 0;
		virtual void startCall(void* fun,const char* name) = 0;
		virtual void endCall(void* fun) = 0;
		virtual SProfileReport* getReport() = 0;
		virtual void report() = 0;
	};

	extern IProfile* Profile;
}
}
#ifdef YON_USE_PROFILE
#define PROFILE_REGISTER_FRAME() yon::debug::Profile->registerFrame()
#define PROFILE_START_CALL(fun) \
	do{yon::c8* n=YON_STR(fun);yon::debug::Profile->startCall(fun,n);}while(0)
#define PROFILE_END_CALL(fun) yon::debug::Profile->endCall(fun)
#define PROFILE_GET_REPORT() yon::debug::Profile->getReport()
#define PROFILE_REPORT() yon::debug::Profile->report()
#else
#define PROFILE_REGISTER_FRAME() 
#define PROFILE_START_CALL(fun) 
#define PROFILE_END_CALL(fun) 
#define PROFILE_GET_REPORT() 
#define PROFILE_REPORT() 
#endif
#endif