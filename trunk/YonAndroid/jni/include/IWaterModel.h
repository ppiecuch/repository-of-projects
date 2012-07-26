#ifndef _YON_SCENE_WATER_IWATERMODEL_H_
#define _YON_SCENE_WATER_IWATERMODEL_H_

#include "IModel.h"

namespace yon{
namespace scene{
namespace water{

	class IWaterModel : public IModel{
	public:
		IWaterModel(IModel* parent,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1))
			:IModel(parent,pos,rot,scale){}
	};
}
}
}
#endif