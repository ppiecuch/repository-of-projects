#ifndef _YON_SCENE_CAMERA_CORTHOCAMERA_H_
#define _YON_SCENE_CAMERA_CORTHOCAMERA_H_

#include "IOrthoCamera.h"

namespace yon{
namespace scene{
namespace camera{
	class COrthoCamera : public IOrthoCamera{
	protected:
		virtual void recalculateProjectionMatrix();
		virtual void recalculateViewMatrix();
		
	public:
		COrthoCamera(IModel* parent=NULL,
			const core::vector3df& pos=core::vector3df(0,0,1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,-1));

		virtual void render(video::IVideoDriver* driver);
		virtual void onRegisterForRender();

		virtual void onResize(const core::dimension2du& size);

		virtual void setNear(f32 nearValue);

		virtual void setFar(f32 farValue);
		
		virtual void setPosition(const core::vector3df& pos);
		virtual void setRotation(const core::vector3df& rot);
	};
}//camera
}//scene
}//yon
#endif