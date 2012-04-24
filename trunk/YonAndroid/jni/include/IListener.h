#ifndef _YON_AUDIO_ILISTENER_H_
#define _YON_AUDIO_ILISTENER_H_

#include "IReferencable.h"
#include "vector3d.h"
#include "position3d.h"

namespace yon{
namespace audio{

	class IListener : public virtual core::IReferencable{
	public:
		virtual ~IListener(){}

		virtual void setGain(f32 gain) = 0;
		virtual f32 getGain() = 0;

		virtual void setPosition(const core::position3df& p) = 0;
		virtual const core::position3df getPosition() = 0;

		virtual void setVelocity(const core::vector3df& v) = 0;
		virtual const core::vector3df getVelocity() = 0;
	};
}
}
#endif