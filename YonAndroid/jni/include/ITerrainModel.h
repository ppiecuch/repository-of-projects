#ifndef _YON_SCENE_TERRAIN_ITERRAINMODEL_H_
#define _YON_SCENE_TERRAIN_ITERRAINMODEL_H_

#include "IModel.h"

namespace yon{
	namespace video{
		class IImage;
	}
namespace scene{
namespace terrain{

	enum ENUM_PATCH_SIZE{
		//! patch size of 9, at most, use 4 levels of detail with this patch size.
		ENUM_PATCH_SIZE_9 = 9,
		//! patch size of 17, at most, use 5 levels of detail with this patch size.
		ENUM_PATCH_SIZE_17 = 17,
		//! patch size of 44, at most, use 6 levels of detail with this patch size.
		ENUM_PATCH_SIZE_33 = 33,
		//! patch size of 65, at most, use 7 levels of detail with this patch size.
		ENUM_PATCH_SIZE_65 = 65,
		//! patch size of 129, at most, use 8 levels of detail with this patch size.
		ENUM_PATCH_SIZE_129 = 129
	};

	class ITerrainModel : public IModel{
	public:
		ITerrainModel(IModel* parent,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1))
			:IModel(parent,pos,rot,scale){}
		virtual f32 getHeight(f32 x,f32 z) const = 0;
		//TODO ¸Ä³Éio::path
		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize=ENUM_PATCH_SIZE_17) = 0;
	};
}
}
}
#endif