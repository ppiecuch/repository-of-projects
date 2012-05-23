#include "CRandomizer.h"

namespace yon{
	CRandomizer::CRandomizer()
		:seed(0x0f0f0f0f){
	}
	void CRandomizer::reset(){
		seed = 0x0f0f0f0f;
	}
	s32 CRandomizer::rand(){
		const s32 m = 2147483399;	// a non-Mersenne prime
		const s32 a = 40692;		// another spectral success story
		const s32 q = m/a;
		const s32 r = m%a;		// again less than q

		seed = a * (seed%q) - r* (seed/q);
		if (seed<0) seed += m;

		return seed;
	}
	s32 CRandomizer::rand(s32 start,s32 end){
		YON_DEBUG_BREAK_IF(end<=start);
		return rand()%(end-start+1)+start;
	}

	IRandomizer* createRandomizer(){
		return new CRandomizer();
	}
}