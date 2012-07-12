#include "CAnimatorFactory.h"
#include "CAnimatorUV.h"
#include "CAnimatorAlpha.h"

namespace yon{
namespace scene{
namespace animator{

	IAnimator* CAnimatorFactory::createAnimator(SAnimatorParam& param){
		switch(param.type){
		case ENUM_ANIMATOR_TYPE_UV:
			{
				core::vector3df translate;
				translate.set(param.animatorUV.translate.u,param.animatorUV.translate.v,param.animatorUV.translate.w);
				return new CAnimatorUV(param.animatorUV.unitIndex,param.animatorUV.stage,translate);
			}
			break;
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
