#ifndef _YON_SCENE_ANIMATOR_CANIMATORFACTORY_H_
#define _YON_SCENE_ANIMATOR_CANIMATORFACTORY_H_

#include "IAnimatorFactory.h"

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorFactory : public IAnimatorFactory{
	public:
		virtual IAnimator* createAnimator(SAnimatorParam& param);
	};
}
}
}

#endif