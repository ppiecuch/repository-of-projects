#ifndef _YON_SCENE_CAMERA_ICAMERA_H_
#define _YON_SCENE_CAMERA_ICAMERA_H_

#include "IRenderable.h"

namespace yon{
	namespace scene{
		namespace camera{
		
			class ICamera : public core::IRenderable{
			public:
				ICamera(const core::vector3df& pos):IRenderable(pos){}

				virtual void setUpVector(const core::vector3df& pos) =0;
				virtual const core::vector3df& getUpVector() const =0;

				virtual f32 getNear() const =0;
				virtual void setNear(f32 near) =0;

				virtual f32 getFar() const =0;
				virtual void setFar(f32 far) =0;

				virtual f32 getAspectRatio() const =0;
				virtual void setAspectRatio(f32 aspect) =0;

				virtual f32 getFovy() const =0;
				virtual void setFovy(f32 fovy) =0;
			};
		}
	}
}
#endif