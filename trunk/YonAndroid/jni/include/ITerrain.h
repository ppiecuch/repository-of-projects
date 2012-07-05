#ifndef _YON_SCENE_TERRAIN_ITERRAIN_H_
#define _YON_SCENE_TERRAIN_ITERRAIN_H_

#include "IEntity.h"

namespace yon{
namespace scene{
namespace terrain{

	class ITerrain : public IEntity{
	public:
		ITerrain(u32 patchsPerSide,s32 patchSize=17);
		virtual f32 getHeight(f32 x,f32 z) const = 0;
		virtual void loadHeightMap() = 0;
	};
}
}
}
#endif