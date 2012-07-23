#ifndef _YON_SCENE_CAMERA_CVIEWFRUSTUM_H_
#define _YON_SCENE_CAMERA_CVIEWFRUSTUM_H_

#include "IViewFrustum.h"

namespace yon{
namespace scene{
namespace camera{

	class CViewFrustum : public IViewFrustum{

		//! the position of the camera
		core::vector3df m_cameraPosition;
		//! all planes enclosing the view frustum.
		core::plane3df m_planes[ENUM_FRUSTUM_PLANE_COUNT];
		//! bounding box around the view frustum
		core::aabbox3df m_boundingBox;
	public:
		virtual void setFrom(const core::matrix4f& mat);

		virtual core::vector3df getFarLeftTop() const;
		virtual core::vector3df getFarLeftBottom() const;
		virtual core::vector3df getFarRightTop() const;
		virtual core::vector3df getFarRightBottom() const;
		virtual core::vector3df getNearLeftTop() const;
		virtual core::vector3df getNearLeftBottom() const;
		virtual core::vector3df getNearRightTop() const;
		virtual core::vector3df getNearRightBottom() const;

		virtual  void recalculateBoundingBox();
		virtual const core::aabbox3df &getBoundingBox() const;

		virtual void render(video::IVideoDriver* driver);
	};
}
}
}
#endif