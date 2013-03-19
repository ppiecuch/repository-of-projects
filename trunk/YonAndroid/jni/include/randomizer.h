#ifndef _YON_CORE_RANDOMIZER_H_
#define _YON_CORE_RANDOMIZER_H_

#include "yonTypes.h"

namespace yon{
namespace core{

	class randomizer{
	private:
		YON_API static s32 seed;
	public:
		static void reset(){
			seed=0x0f0f0f0f;
		}

		static s32 rand(){
			const static s32 m = 2147483399;	// a non-Mersenne prime
			const static s32 a = 40692;			// another spectral success story
			const static s32 q = m/a;
			const static s32 r = m%a;			// again less than q

			seed = a * (seed%q) - r* (seed/q);
			if (seed<0) seed += m;

			return seed;
		}

		//获取随机数，范围为[start,end]
		static s32 rand(s32 start,s32 end){
			if(start==end)
				return start;
			YON_DEBUG_BREAK_IF(end<start);
			return rand()%(end-start+1)+start;
		}
	};
}
}
#endif