#include "lcRandomizer.h"

namespace lc{

	template<> randomizer* core::Singleton<randomizer>::s_pSingleton = NULL;

	randomizer::randomizer():m_iSeed(0){
		reset();
	}

	void randomizer::reset(){
		m_iSeed=0x0f0f0f0f;
	}
	s32 randomizer::rand(){
		const static s32 m = 2147483399;	// a non-Mersenne prime
		const static s32 a = 40692;			// another spectral success story
		const static s32 q = m/a;
		const static s32 r = m%a;			// again less than q

		//TODO need mutex?

		m_iSeed = a * (m_iSeed%q) - r* (m_iSeed/q);
		if (m_iSeed<0) m_iSeed += m;

		return m_iSeed;
	}
	s32 randomizer::rand(s32 start,s32 end){
		if(start==end)
			return start;
		LC_DEBUG_BREAK_IF(end<start);
		return rand()%(end-start+1)+start;
	}
}