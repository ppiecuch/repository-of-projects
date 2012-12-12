#ifndef _YON_SCENE_IANIMATIONENDCALLBACK_H_
#define _YON_SCENE_IANIMATIONENDCALLBACK_H_

#include "IReferencable.h"

namespace yon{
namespace scene{

	class IAnimatedSceneNode;

	//! Callback interface for catching events of ended animations.
	/** Implement this interface and use
	IAnimatedMeshSceneNode::setAnimationEndCallback to be able to
	be notified if an animation playback has ended.
	**/
	class IAnimationEndCallBack : public virtual core::IReferencable
	{
	public:

		//! Will be called when the animation playback has ended.
		/** See IAnimatedSceneNode::setAnimationEndCallback for
		more informations.
		\param node: Node of which the animation has ended. */
		virtual void onAnimationEnd(IAnimatedSceneNode* node) = 0;
	};

}
}
#endif