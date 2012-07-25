#ifndef _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN2_H_
#define _YON_SCENE_TERRAIN_CGEOMIPMAPTERRAIN2_H_

#include "ITerrainModel.h"
#include "IUnit.h"
#include "SDynamicShap.h"
#include "aabbox3d.h"

namespace yon{
namespace scene{
namespace terrain{

	class CGeomipmapTerrain2 : public ITerrainModel{
	private:

		//3d coordinate:
		//             z index
		//             ¡ü ¡ü
		//    right    ¨O ¨O
		//  top©àbottom¨O ¨O
		//    left     ¨O ¨O
		//x¡û©¤©¤©¤©¤©¤©¼
		//
		//image coordinate:
		// z           
		// ¡ü
		// ¨O
		// ¨O
		// ¨O
		// ©¸©¤©¤©¤©¤©¤¡úx
		//the axis z in 3d corresponds to the axis x in image coordinate, that is, 
		//rotate the image coordinate 90 degree in CCW will make the twos coordinates be coincident.
		struct SPatch{
			SPatch():m_iLOD(-1),top(NULL),bottom(NULL),left(NULL),right(NULL),
				m_boundingBox(core::aabbox3df(true))
			{}
			core::vector3df m_centerPos;
			core::aabbox3df	m_boundingBox;
			s16 m_iLOD;

			SPatch* top;
			SPatch* bottom;
			SPatch* left;
			SPatch* right;
		};

		IUnit* m_pUnit;
		//typedef SDynamicShap<SVertex2TCoords,u32,ENUM_VERTEX_TYPE_3V2T1C> SDynamicShap3D2T32;
		SDynamicShap3D2T m_shap;

		SPatch* m_pPatchs;
		s32 m_iMaxLOD;
		s32 m_iSizePerSide;
		s32 m_iPatchSize;
		s32 m_iPatchCountPerSide;
		core::array<f64> m_distanceThresholds;
		core::aabbox3df	m_boundingBox;

		//calculate or recalculate the distance thresholds
		void calculateDistanceThresholds();
		void createPatches();
		void calculatePatchData();

		bool preRenderLODCalculations();
		void preRenderIndicesCalculations();
		u32 getIndex(const s32 PatchX, const s32 PatchZ,const s32 PatchIndex, u32 vX, u32 vZ) const;
	public:
		CGeomipmapTerrain2(IModel* parent,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale);
		~CGeomipmapTerrain2();

		virtual void loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize);

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender();

		virtual f32 getHeight(f32 x,f32 z) const;

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