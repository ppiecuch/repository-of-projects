#ifndef _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_
#define _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"

namespace yon{
namespace scene{
namespace terrain{

	class CGeomipmapTerrain : public ITerrainModel{
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

		IUnit* m_pUnit;
		SDynamicShap3D2T m_shap;

		SPatch* m_pPatchs;
		core::vector3df m_scaleSize;
		s32 m_iMaxLOD;
		s32 m_iSizePerSide;
		s32 m_iPatchSize;
		s32 m_iPatchCountPerSide;
		core::array<f64> m_distanceThresholds;

		//calculate or recalculate the distance thresholds
		void calculateDistanceThresholds();
		void createPatches();
		void calculatePatchData();

		void calculateIndices();
		u32 getIndex(const s32 PatchX, const s32 PatchZ,const s32 PatchIndex, u32 vX, u32 vZ) const;
	public:
		CGeomipmapTerrain(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CGeomipmapTerrain();

		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize);

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender(ISceneManager* manager);

		virtual f32 getHeight(f32 x,f32 z) const{
			//TODO
			return 0;
		}

		virtual u32 getMaterialCount() const{
			return 1;
		}

		virtual video::IMaterial* getMaterial(u32 num) const{
			if(num>=getMaterialCount())
				return NULL;
			return m_pUnit->getMaterial();
		}
	};
}
}
}
#endif