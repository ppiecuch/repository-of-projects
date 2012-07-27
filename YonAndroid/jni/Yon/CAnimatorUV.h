#ifndef _YON_SCENE_ANIMATOR_CANIMATORUV_H_
#define _YON_SCENE_ANIMATOR_CANIMATORUV_H_

#include "IAnimator.h"
#include "IMaterial.h"
#include "IModel.h"

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorUV : public IAnimator{
	private:
		u32 m_uUnitIndex;
		u32 m_uStage;
		core::vector3df m_translate;

		u32 m_uLastTime;
		bool m_bFirstUpdated;
	public:
		CAnimatorUV(u32 unitIndex,u32 stage,const core::vector3df& translate):m_uUnitIndex(unitIndex),m_uStage(stage),m_translate(translate){}

		virtual void animateNode(IModel* model, u32 timeMs){

			if(m_bFirstUpdated)
			{
				m_uLastTime=timeMs;
				m_bFirstUpdated=false;
			}

			f32 timeDiff = (f32)(timeMs-m_uLastTime)/1000.f;
			m_uLastTime=timeMs;

			video::IMaterial* material=model->getMaterial(m_uUnitIndex);
			core::matrix4f& mat=material->getTextureMatrix(m_uStage);
			mat.translate(m_translate.x*timeDiff,m_translate.y*timeDiff,m_translate.z*timeDiff);
		}
		virtual ENUM_ANIMATOR_TYPE getType() const{
			return ENUM_ANIMATOR_TYPE_UV;
		}
	};
}
}
}
#endif