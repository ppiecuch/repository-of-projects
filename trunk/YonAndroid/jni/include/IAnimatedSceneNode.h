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
		
		//! Sets looping mode which is on by default.
		/** If set to false, animations will not be played looped. */
		virtual void setLoopMode(bool playAnimationLooped) = 0;
		
		//! Sets the frame numbers between the animation is looped.
		/** The default is 0 - MaximalFrameCount of the mesh.
		\param begin: Start frame number of the loop.
		\param end: End frame number of the loop.
		\return True if successful, false if not. */
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;

		//! Sets the speed with which the animation is played.
		/** \param framesPerSecond: Frames per second played. */
		virtual void setAnimationSpeed(f32 framesPerSecond) = 0;

		//! Gets the speed with which the animation is played.
		/** \return Frames per second played. */
		virtual f32 getAnimationSpeed() const =0;
		
		//! Sets a new mesh
		virtual void setEntity(IAnimatedEntity* entity) = 0;

		virtual void setCurrentFrame(f32 frame) = 0;
		virtual f32 getCurrentFrame() = 0;
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;
		virtual void setAnimationSpeed(f32 framesPerSecond) = 0;
		virtual f32 getAnimationSpeed() const =0;

		//! animates the joints in the mesh based on the current frame.
		/** Also takes in to account transitions. */
		virtual void animateJoints(bool CalculateAbsolutePositions=true) = 0;
		
		//! Get a pointer to a joint in the mesh (if the mesh is a bone based mesh).
		/** With this method it is possible to attach scene nodes to
		joints for example possible to attach a weapon to the left hand
		of an animated model. This example shows how:
		\code
		ISceneNode* hand =
			yourAnimatedMeshSceneNode->getJointNode("LeftHand");
		hand->addChild(weaponSceneNode);
		\endcode
		Please note that the joint returned by this method may not exist
		before this call and the joints in the node were created by it.
		\param jointName: Name of the joint.
		\return Pointer to the scene node which represents the joint
		with the specified name. Returns 0 if the contained mesh is not
		an skinned mesh or the name of the joint could not be found. */
		virtual IBoneSceneNode* getJointNode(const c8* jointName)=0;

		//! same as getJointNode(const c8* jointName), but based on id
		//virtual IBoneSceneNode* getJointNode(u32 jointID) = 0;

		//virtual bool attachSceneNodeToJoint(const core::stringc id,const IModel* node,const core::stringc jointName) = 0;
		//virtual bool detachSceneNode(const core::stringc id) = 0;
	};
}
}
#endif