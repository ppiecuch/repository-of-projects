#ifndef _YON_SCENE_IDUMMYSCENENODE_H_
#define _YON_SCENE_IDUMMYSCENENODE_H_

#include "ISceneNode.h"

namespace yon{
namespace scene{
	
	//! Dummy scene node for adding additional transformations to the scene graph.
	/** This scene node does not render itself, and does not respond to set/getPosition,
	set/getRotation and set/getScale. Its just a simple scene node that takes a
	matrix as relative transformation, making it possible to insert any transformation
	anywhere into the scene graph.
	This scene node is for example used by the IAnimatedSceneNode for emulating
	joint scene nodes when playing skeletal animations.
	*/
	class IDummySceneNode : public ISceneNode{
	public:
		//! Returns a reference to the current relative transformation matrix.
		/** This is the matrix, this scene node uses instead of scale, translation
		and rotation. */
		virtual core::matrix4f& getRelativeTransformationMatrix() = 0;
	};
}
}
#endif