#ifndef _YON_IRANDOMIZER_H_
#define _YON_IRANDOMIZER_H_

#include "IReferencable.h"

namespace yon{
	class IRandomizer : public virtual core::IReferencable{
	public:
		virtual void reset() = 0;
		virtual s32 rand() = 0;
		//获取随机数，范围为[start,end]
		virtual s32 rand(s32 start,s32 end) = 0;
	};
}
#endif