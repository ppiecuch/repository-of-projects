#ifndef _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_
#define _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_

#include "ITerrain.h"
#include "SUnit.h"
#include "SDynamicShap.h"

namespace yon{
namespace scene{
namespace terrain{

	class CGeomipmapTerrain : public ITerrain{
	private:
		struct SPatch{
			SPatch():m_iLOD(-1),top(NULL),bottom(NULL),left(NULL),right(NULL)
			{}
			f32 m_fDistance;
			s16 m_iLOD;

			SPatch* top;
			SPatch* bottom;
			SPatch* left;
			SPatch* right;
		};

		Unit3D m_unit;
		SDynamicShap3D2T m_shap;

		SPatch* m_pPatchs;
		s32 m_iMaxLOD;
		s32 m_iSizePerSide;
		s32 m_iPatchSize;
		s32 m_iPatchCountPerSide;

	public:
		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize);

		virtual f32 getHeight(f32 x,f32 z) const{
			//TODO
			return 0;
		}
	};
}
}
}
#endif