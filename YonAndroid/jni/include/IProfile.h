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
			u64 NTimeConsumeSum;
			u64 NTimeConsumeMin;
			u64 NTimeConsumeMax;
			u64 NTimeConsumeAvg;
			f32 NTimeConsumePct;
			u64 _calltime;
		};
		u32 FrameCount;
		u64 TimeDiff;
		f32 FPS;
		core::map<u32,SAPIReport*> ApiInfos;
	};

	class IProfile : public core::IReferencable{
	public:
		virtual void registerFrame() = 0;
		virtual void startCall(u32 id,const char* name) = 0;
		virtual void endCall(u32 id) = 0;
		virtual SProfileReport* getReport() = 0;
		virtual void report() = 0;
	};

	YON_API extern IProfile* Profile;
}
}
#ifdef YON_USE_PROFILE
#define PROFILE_REGISTER_FRAME() yon::debug::Profile->registerFrame()
//TODO 不支持类成员函数
//#define PROFILE_START_CALL(fun) \
//	do{yon::c8* n=YON_STR(fun);yon::debug::Profile->startCall(reinterpret_cast<void*>((size_t)fun),n);}while(0)
//#define PROFILE_END_CALL(fun) yon::debug::Profile->endCall(fun)
#define PROFILE_START_CALL(id,fun) \
	do{const yon::c8* n=YON_STR(fun);yon::debug::Profile->startCall(id,n);}while(0)
#define PROFILE_END_CALL(id) yon::debug::Profile->endCall(id)
#define PROFILE_GET_REPORT() yon::debug::Profile->getReport()
#define PROFILE_REPORT() yon::debug::Profile->report()
#else
#define PROFILE_REGISTER_FRAME() 
//#define PROFILE_START_CALL(fun) 
//#define PROFILE_END_CALL(fun) 
#define PROFILE_START_CALL(id,fun) 
#define PROFILE_END_CALL(id) 
#define PROFILE_GET_REPORT() 
#define PROFILE_REPORT() 
#endif

#define PROFILE_ID_1		0x10000000
#define PROFILE_ID_1_1		0x11000000
#define PROFILE_ID_1_2		0x12000000
#define PROFILE_ID_1_3		0x13000000
#define PROFILE_ID_1_4		0x14000000
#define PROFILE_ID_1_5		0x15000000
#define PROFILE_ID_1_6		0x16000000
#define PROFILE_ID_1_7		0x17000000
#define PROFILE_ID_1_8		0x18000000

#define PROFILE_ID_2		0x20000000
#define PROFILE_ID_2_1		0x21000000
#define PROFILE_ID_2_2		0x22000000
#define PROFILE_ID_2_3		0x23000000
#define PROFILE_ID_2_4		0x24000000
#define PROFILE_ID_2_5		0x25000000
#define PROFILE_ID_2_6		0x26000000
#define PROFILE_ID_2_7		0x27000000
#define PROFILE_ID_2_8		0x28000000

#define PROFILE_ID_3		0x30000000
#define PROFILE_ID_3_1		0x31000000
#define PROFILE_ID_3_2		0x32000000
#define PROFILE_ID_3_3		0x33000000
#define PROFILE_ID_3_4		0x34000000
#define PROFILE_ID_3_5		0x35000000
#define PROFILE_ID_3_6		0x36000000
#define PROFILE_ID_3_7		0x37000000
#define PROFILE_ID_3_8		0x38000000

#define PROFILE_ID_4		0x40000000
#define PROFILE_ID_4_1		0x41000000
#define PROFILE_ID_4_2		0x42000000
#define PROFILE_ID_4_3		0x43000000
#define PROFILE_ID_4_4		0x44000000
#define PROFILE_ID_4_5		0x45000000
#define PROFILE_ID_4_6		0x46000000
#define PROFILE_ID_4_7		0x47000000
#define PROFILE_ID_4_8		0x48000000

#define PROFILE_ID_5		0x50000000
#define PROFILE_ID_5_1		0x51000000
#define PROFILE_ID_5_2		0x52000000
#define PROFILE_ID_5_3		0x53000000
#define PROFILE_ID_5_4		0x54000000
#define PROFILE_ID_5_5		0x55000000
#define PROFILE_ID_5_6		0x56000000
#define PROFILE_ID_5_7		0x57000000
#define PROFILE_ID_5_8		0x58000000

#define PROFILE_ID_6		0x60000000
#define PROFILE_ID_6_1		0x61000000
#define PROFILE_ID_6_2		0x62000000
#define PROFILE_ID_6_3		0x63000000
#define PROFILE_ID_6_4		0x64000000
#define PROFILE_ID_6_5		0x65000000
#define PROFILE_ID_6_6		0x66000000
#define PROFILE_ID_6_7		0x67000000
#define PROFILE_ID_6_8		0x68000000

#define PROFILE_ID_7		0x70000000
#define PROFILE_ID_7_1		0x71000000
#define PROFILE_ID_7_2		0x72000000
#define PROFILE_ID_7_3		0x73000000
#define PROFILE_ID_7_4		0x74000000
#define PROFILE_ID_7_5		0x75000000
#define PROFILE_ID_7_6		0x76000000
#define PROFILE_ID_7_7		0x77000000
#define PROFILE_ID_7_8		0x78000000

#define PROFILE_ID_8		0x80000000
#define PROFILE_ID_8_1		0x81000000
#define PROFILE_ID_8_2		0x82000000
#define PROFILE_ID_8_3		0x83000000
#define PROFILE_ID_8_4		0x84000000
#define PROFILE_ID_8_5		0x85000000
#define PROFILE_ID_8_6		0x86000000
#define PROFILE_ID_8_7		0x87000000
#define PROFILE_ID_8_8		0x88000000
#endif