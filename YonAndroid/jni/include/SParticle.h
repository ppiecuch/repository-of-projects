#ifndef _YON_SCENE_EFFECT_SPARTICLE_H_
#define _YON_SCENE_EFFECT_SPARTICLE_H_

#include "vector3d.h"
#include "dimension2d.h"
#include "SColor.h"

namespace yon{
namespace scene{
namespace effect{

	//TODO 关键帧粒子系统：http://www.gamenpc.com/Forum1/Topic/225.aspx

	struct SParticle{

		//! Position of the particle
		core::vector3df Pos;

		//! Direction and speed of the particle
		core::vector3df Vector;

		//! Start life time of the particle
		u32 StartTime;

		//! End life time of the particle
		u32 EndTime;

		//! Current color of the particle
		video::SColor Color;

		//! Original color of the particle.
		/** That's the color of the particle it had when it was emitted. */
		video::SColor StartColor;

		//! Original direction and speed of the particle.
		/** The direction and speed the particle had when it was emitted. */
		core::vector3df StartVector;

		//! Scale of the particle.
		/** The current scale of the particle. */
		core::dimension2df Size;

		//! Original scale of the particle.
		/** The scale of the particle when it was emitted. */
		core::dimension2df StartSize;
	};
}
}
}
#endif