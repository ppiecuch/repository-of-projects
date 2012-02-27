#ifndef _YON_ITIMER_H_
#define _YON_ITIMER_H_

#include "IReferencable.h"
#include "types.h"

namespace yon{
	class ITimer : public virtual core::IReferencable{
	public:
		virtual ~ITimer(){};
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void setSpeed(f32 speed=1.0f) = 0;
		virtual u32 getTime() const = 0;
		virtual void setTime(u32 time) = 0;
		virtual void tick() = 0;
	};
}
#endif