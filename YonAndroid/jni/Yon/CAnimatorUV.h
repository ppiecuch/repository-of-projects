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
	public:
		CAnimatorUV(u32 unitIndex,u32 stage):m_uUnitIndex(unitIndex),m_uStage(stage){}

		virtual void animateNode(IModel* model, u32 timeMs){
			video::IMaterial* material=model->getMaterial(m_uUnitIndex);
			core::matrix4f& mat=material->getTextureMatrix(m_uStage);
			mat.translate(0,0.01f,0);
		}
		virtual ENUM_ANIMATOR_TYPE getType() const{
			return ENUM_ANIMATOR_TYPE_UV;
		}
	};
}//animator
}//scene
}//yon
#endif