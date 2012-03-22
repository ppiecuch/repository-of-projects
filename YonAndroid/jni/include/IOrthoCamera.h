#ifndef _YON_SCENE_CAMERA_IORTHOCAMERA_H_
#define _YON_SCENE_CAMERA_IORTHOCAMERA_H_

#include "ICamera.h"

namespace yon{
namespace scene{
namespace camera{

	class IOrthoCamera : public ICamera{
	public:

		virtual void setLeft(f32 left) = 0;
		virtual f32 getLeft() const = 0;
		virtual void setRight(f32 right) = 0;
		virtual f32 getRight() const = 0;
		virtual void setTop(f32 top) = 0;
		virtual f32 getTop() const = 0;
		virtual void setBottom(f32 bottom) = 0;
		virtual f32 getBottom() const = 0;

		virtual ENUM_CAMERA_TYPE getType() const{
			return ENUM_CAMERA_TYPE_ORTHO;
		}
	};
}//camera
}//scene
}//yon
#endif