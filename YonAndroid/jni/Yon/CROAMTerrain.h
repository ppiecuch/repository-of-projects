#ifndef _YON_SCENE_TERRAIN_CROAMTERRAIN_H_
#define _YON_SCENE_TERRAIN_CROAMTERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

	class CROAMTerrain : public ITerrainModel{
	private:
	public:
		CROAMTerrain(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CROAMTerrain();
	};
}
}
}
#endif