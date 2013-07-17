#ifndef _LUCID_RANDOM_H_
#define _LUCID_RANDOM_H_

#include "lcSingleton.h"

namespace lc{

	class randomizer : public core::Singleton<randomizer>{
	private:
		s32 m_iSeed;
		randomizer();
		friend class core::Singleton<randomizer>;
	public:
		void reset();
		s32 rand();
		s32 rand(s32 start,s32 end);
	};
}
#endif