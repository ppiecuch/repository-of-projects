#ifndef _YON_ITIMER_H_
#define _YON_ITIMER_H_

#include "IReferencable.h"
#include "yonTypes.h"

namespace yon{
	class ITimer : public virtual core::IReferencable{
	public:
		virtual ~ITimer(){};
		virtual void start() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual bool isPaused() = 0;
		virtual void setSpeed(f32 speed=1.0f) = 0;
		virtual u32 getTime() const = 0;
		virtual u32 getRealTime() = 0;
		virtual void tick() = 0;
	};

	ITimer* createTimer();
}
#endif