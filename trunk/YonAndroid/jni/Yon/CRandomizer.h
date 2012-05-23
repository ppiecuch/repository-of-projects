#ifndef _YON_CRANDOMIZER_H_
#define _YON_CRANDOMIZER_H_

#include "IRandomizer.h"

namespace yon{
	class CRandomizer : public IRandomizer{
	protected:
		s32 seed;
	public:
		CRandomizer();
		virtual void reset();
		virtual s32 rand();
		virtual s32 rand(s32 start,s32 end);
	};

	IRandomizer* createRandomizer();
}
#endif