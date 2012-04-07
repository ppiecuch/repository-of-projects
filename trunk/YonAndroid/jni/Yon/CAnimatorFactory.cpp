#include "CAnimatorFactory.h"
#include "CAnimatorUV.h"

namespace yon{
namespace scene{
namespace animator{

	IAnimator* CAnimatorFactory::createAnimator(SAnimatorParam& param){
		switch(param.type){
		case ENUM_ANIMATOR_TYPE_UV:
			return new CAnimatorUV(param.animatorUV.unitIndex,param.animatorUV.stage);
		}
		return NULL;
	}

	IAnimatorFactory* createAnimatorFactory(){
		return new CAnimatorFactory();
	}
}
}
}
