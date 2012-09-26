#ifndef _YON_SCENE_IANIMATEDSCENENODE_H_
#define _YON_SCENE_IANIMATEDSCENENODE_H_

#include "IModel.h"
#include "yonString.h"

namespace yon{
namespace scene{

	class IAnimatedSceneNode : public IModel{
	public:
		IAnimatedSceneNode(IModel* parent,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)):
		IModel(parent,pos,rot,scale){}

		virtual ~IAnimatedSceneNode(){}

		virtual void setCurrentFrame(f32 frame) = 0;
		virtual f32 getCurrentFrame() = 0;
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;
		virtual void setAnimationSpeed(f32 framesPerSecond) = 0;
		virtual f32 getAnimationSpeed() const =0;

		//! animates the joints in the mesh based on the current frame.
		/** Also takes in to account transitions. */
		//virtual void animateJoints(bool CalculateAbsolutePositions=true) = 0;
		virtual void animate() = 0;

		virtual bool attachSceneNodeToJoint(const core::stringc id,const IModel* node,const core::stringc jointName) = 0;
		virtual bool detachSceneNode(const core::stringc id) = 0;
	};
}
}
#endif