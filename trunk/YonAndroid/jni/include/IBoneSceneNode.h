#ifndef _YON_SCENE_IBONESCENENODE_H_
#define _YON_SCENE_IBONESCENENODE_H_

#include "IModel.h"

namespace yon{
namespace scene{

	//! Interface for bones used for skeletal animation.
	/** Used with ISkinnedEntity and IAnimatedSceneNode. */
	class IBoneSceneNode : public IModel{
	public:
		//! Get the index of the bone
		virtual u32 getBoneIndex() const = 0;

		//! The animation method.
		virtual void onAnimate(u32 timeMs) =0;

		//! The render method.
		/** Does nothing as bones are not visible. */
		virtual void render() {}

		//! Updates the absolute position based on the relative and the parents position
		virtual void updateAbsolutePositionOfAllChildren()=0;

		//! Get the axis aligned bounding box of this node
		//virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;
	};
}
}

#endif