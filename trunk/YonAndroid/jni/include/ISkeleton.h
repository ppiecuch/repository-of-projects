#ifndef _YON_SCENE_ISKELETON_H_
#define _YON_SCENE_ISKELETON_H_

#include "IReferencable.h"

namespace yon{
namespace scene{

	class ISkeleton : public core::IReferencable{
	private:
		//! Animation keyframe which describes a new position
		struct SPositionKey
		{
			f32 frame;
			core::vector3df position;
		};

		//! Animation keyframe which describes a new scale
		struct SScaleKey
		{
			f32 frame;
			core::vector3df scale;
		};

		//! Animation keyframe which describes a new rotation
		struct SRotationKey
		{
			f32 frame;
			core::quaternion rotation;
		};
	public:
		virtual u32 getJointCount() const = 0;
	};
}
}
#endif