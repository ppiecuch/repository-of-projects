#ifndef _YON_SCENE_CAMERA_ICAMERA_H_
#define _YON_SCENE_CAMERA_ICAMERA_H_

#include "IRenderable.h"

namespace yon{
	namespace scene{
		namespace camera{

			enum ENUM_CAMERA_TYPE{
				ENUM_CAMERA_TYPE_PERSP = 0,
				ENUM_CAMERA_TYPE_ORTHO,
				ENUM_CAMERA_TYPE_COUNT
			};

			enum ENUM_VIEW_PLANE{
				ENUM_VIEW_PLANE_FAR = 0,
				ENUM_VIEW_PLANE_NEAR,
				ENUM_VIEW_PLANE_LEFT,
				ENUM_VIEW_PLANE_RIGHT,
				ENUM_VIEW_PLANE_TOP,
				ENUM_VIEW_PLANE_BOTTOM,
				ENUM_VIEW_PLANE_COUNT
			};
		
			class ICamera : public core::IRenderable{
			protected:
				bool m_needRender;
			public:
				ICamera(const core::vector3df& pos,const core::vector3df& rot,const core::vector3df& scale,bool needRender=false):
				IRenderable(pos,rot,scale),m_needRender(needRender){}

				virtual void setUpVector(const core::vector3df& pos) =0;
				virtual const core::vector3df& getUpVector() const =0;

				virtual f32 getNear() const =0;
				virtual void setNear(f32 near) =0;

				virtual f32 getFar() const =0;
				virtual void setFar(f32 far) =0;

				virtual void setNeedRender(bool on){
					m_needRender=on;
				}
				virtual bool getNeedRender() const{
					return m_needRender;
				}

				virtual ENUM_CAMERA_TYPE getType() const = 0;
			};
		}
	}
}
#endif