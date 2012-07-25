#ifndef _YON_SCENE_CAMERA_CPERSPCAMERA_H_
#define _YON_SCENE_CAMERA_CPERSPCAMERA_H_

#include "IPerspCamera.h"
#include "CViewFrustum.h"

namespace yon{
namespace scene{
namespace camera{
	class CPerspCamera : public IPerspCamera{
	protected:
		virtual void recalculateProjectionMatrix();
		virtual void recalculateViewMatrix();

		CViewFrustum m_frustum;
	public:
		CPerspCamera(IModel* parent=NULL,
			const core::vector3df& pos=core::vector3df(0,0,1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,-1));

		virtual void setAspectRatio(f32 aspect);
		virtual void setFovy(f32 fovy);

		virtual void render(video::IVideoDriver* driver);
		virtual void onRegisterForRender();

		virtual void onResize(const core::dimension2du& size);

		virtual void setNear(f32 nearValue);

		virtual void setFar(f32 farValue);

		virtual void setPosition(const core::vector3df& pos);
		virtual void setRotation(const core::vector3df& rot);
		virtual void setTarget(const core::vector3df& target);

		virtual IViewFrustum* getViewFrustum(){return &m_frustum;}
	};
}
}
}
#endif