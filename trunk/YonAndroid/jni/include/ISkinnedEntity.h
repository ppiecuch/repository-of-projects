#ifndef _YON_SCENE_ISKINNEDENTITY_H_
#define _YON_SCENE_ISKINNEDENTITY_H_

#include "IAnimatedEntity.h"

namespace yon{
namespace scene{

	class ISkeleton;

	class ISkinnedEntity : public IAnimatedEntity{
	public:

		virtual void skin() = 0;
		virtual bool attachSkeleton(ISkeleton* skeleton) = 0;
	};
}
}
#endif