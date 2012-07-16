#ifndef _YON_SCENE_ANIMATOR_CANIMATORROTATE_H_
#define _YON_SCENE_ANIMATOR_CANIMATORROTATE_H_

#include "IAnimator.h"

namespace yon{
namespace scene{
namespace animator{

	class CAnimatorRotate : public IAnimator{
		f32 m_fRotateSpeed;
		u32 m_uStartTime;
		core::vector3df m_originRotation;
	public:
		CAnimatorRotate(f32 rotateSpeed):m_fRotateSpeed(rotateSpeed),m_uStartTime(0){}

		virtual void animateNode(IModel* model, u32 timeMs){
			if(m_uStartTime==0)
			{
				m_uStartTime=timeMs;
				m_originRotation=model->getRotation();
			}
			f32 degree=m_fRotateSpeed*(timeMs-m_uStartTime);
			core::vector3df tmp=m_originRotation;
			tmp.y+=degree;
			//TODO ²¹È«
			model->setRotation(tmp);
		}
		virtual ENUM_ANIMATOR_TYPE getType() const{
			return ENUM_ANIMATOR_TYPE_ROTATE;
		}
	};
}
}
}
#endif