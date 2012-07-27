#ifndef _YON_SCENE_WATER_CWATERMODEL_H_
#define _YON_SCENE_WATER_CWATERMODEL_H_

#include "IWaterModel.h"
#include "SDynamicShap.h"

namespace yon{
namespace scene{
namespace water{

	class CWaterModel : public IWaterModel{
	private:
		s32 m_iWidth,m_iHeight;
		IUnit* m_pUnit;
		SDynamicShap3D m_shap;

		s32 getIndex(s32 px,s32 pz,s32 vx,s32 vz);
	public:
		CWaterModel(IModel* parent,s32 xsize,s32 zsize,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1));
		~CWaterModel();

		virtual void render(video::IVideoDriver* driver);

		virtual void onRegisterForRender();

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