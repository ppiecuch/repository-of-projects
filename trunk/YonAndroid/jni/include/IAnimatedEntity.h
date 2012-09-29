#ifndef _YON_SCENE_IANIMATEDENTITY_H_
#define _YON_SCENE_IANIMATEDENTITY_H_

#include "IEntity.h"

namespace yon{
namespace scene{

	class IAnimatedEntity : public IEntity{
	public:
		//! Gets the frame count of the animated entity.
		/** \return Returns the amount of frames. If the amount is 1,
		it is a static, non animated mesh. */
		virtual u32 getFrameCount() const = 0;

		//! Returns the IEntity interface for a frame.
		/** \param frame: Frame number as zero based index. The maximum
		frame number is getFrameCount() - 1;
		\param detailLevel: Level of detail. 0 is the lowest, 255 the
		highest level of detail. Most meshes will ignore the detail level.
		\param startFrameLoop: Because some animated meshes (.MD2) are
		blended between 2 static frames, and maybe animated in a loop,
		the startFrameLoop and the endFrameLoop have to be defined, to
		prevent the animation to be blended between frames which are
		outside of this loop.
		If startFrameLoop and endFrameLoop are both -1, they are ignored.
		\param endFrameLoop: see startFrameLoop.
		\return Returns the animated mesh based on a detail level. */
		virtual IEntity* getEntity(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1) = 0;
	};
}
}
#endif