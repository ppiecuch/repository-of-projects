#ifndef _YON_SCENE_ANIMATOR_IANIMATORCAMERAFPS_H_
#define _YON_SCENE_ANIMATOR_IANIMATORCAMERAFPS_H_

#include "IAnimator.h"

namespace yon{
	namespace event{
		struct SKeyMap;
	}
namespace scene{
namespace animator{

	class IAnimatorCameraFPS : public IAnimator{
	public:
		//! Returns the speed of movement in units per millisecond
		virtual f32 getMoveSpeed() const = 0;
		//! Sets the speed of movement in units per millisecond
		virtual void setMoveSpeed(f32 moveSpeed) = 0;

		//! Sets the keyboard mapping for this animator
		/** \param map Array of keyboard mappings, see event::SKeyMap
		\param count Size of the keyboard map array. */
		virtual void setKeyMap(event::SKeyMap *map, s32 count) = 0;

		virtual ENUM_ANIMATOR_TYPE getType() const{
			return ENUM_ANIMATOR_TYPE_FPSCAMERA;
		}

		virtual bool isEventReceivable() const{return true;}
	};
}
}
}
#endif