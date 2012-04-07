#ifndef _YON_SCENE_ANIMATOR_IANIMATORFACTORY_H_
#define _YON_SCENE_ANIMATOR_IANIMATORFACTORY_H_

#include "IReferencable.h"
#include "IAnimator.h"

namespace yon{
namespace scene{
namespace animator{
			
	class IAnimatorFactory : public virtual core::IReferencable{
	public:
		virtual IAnimator* createAnimator(SAnimatorParam& param) = 0;
	};

	IAnimatorFactory* createAnimatorFactory();
}
}
}
#endif