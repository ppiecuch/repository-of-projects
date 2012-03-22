#ifndef _YON_SCENE_CAMERA_IPERSPCAMERA_H_
#define _YON_SCENE_CAMERA_IPERSPCAMERA_H_

#include "ICamera.h"

namespace yon{
namespace scene{
namespace camera{

	class IPerspCamera : public ICamera{
	public:

		virtual f32 getAspectRatio() const =0;
		virtual void setAspectRatio(f32 aspect) =0;

		virtual f32 getFovy() const =0;
		virtual void setFovy(f32 fovy) =0;

		virtual ENUM_CAMERA_TYPE getType() const{
			return ENUM_CAMERA_TYPE_PERSP;
		}

	};
}//camera
}//scene
}//yon
#endif