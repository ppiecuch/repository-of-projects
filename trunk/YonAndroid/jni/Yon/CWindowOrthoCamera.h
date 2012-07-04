#ifndef _YON_SCENE_CAMERA_CWINDOWCAMERA_H_
#define _YON_SCENE_CAMERA_CWINDOWCAMERA_H_

#include "COrthoCamera.h"
#include "position3d.h"

namespace yon{
namespace scene{
namespace camera{

	class CWindowOrthoCamera : public COrthoCamera{
	protected:
		virtual void recalculateViewMatrix();
		//偏移向量，将原点从世界原点偏移到视口第一象限中心
		core::position3df m_originalPos;
	public:
		CWindowOrthoCamera(
			const core::vector3df& pos=core::vector3df(0,0,1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,-1));

		virtual void onResize(const core::dimension2du& size);

		virtual ENUM_CAMERA_TYPE getType() const{
			return ENUM_CAMERA_TYPE_ORTHO_WINDOW;
		}
	};
}
}
}
#endif