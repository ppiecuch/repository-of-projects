#include "CAnimatorFactory.h"
#include "CAnimatorUV.h"
#include "CAnimatorAlpha.h"

namespace yon{
namespace scene{
namespace animator{

	IAnimator* CAnimatorFactory::createAnimator(SAnimatorParam& param){
		switch(param.type){
		case ENUM_ANIMATOR_TYPE_UV:
			return new CAnimatorUV(param.animatorUV.unitIndex,param.animatorUV.stage);
		case ENUM_ANIMATOR_TYPE_ALPHA:
			return new CAnimatorAlpha(param.animatorAlpha.unitIndex,param.animatorAlpha.minValue,param.animatorAlpha.maxValue,param.animatorAlpha.increment);
		}
		return NULL;
	}

	IAnimatorFactory* createAnimatorFactory(){
		return new CAnimatorFactory();
	}
}
}
}
