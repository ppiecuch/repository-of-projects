#ifndef _YON_SCENE_IANIMATEDENTITY_H_
#define _YON_SCENE_IANIMATEDENTITY_H_

#include "IEntity.h"

namespace yon{
namespace scene{

	class IAnimatedEntity : public IEntity{
	public:
		virtual u32 getFrameCount() const = 0;
	};
}
}
#endif