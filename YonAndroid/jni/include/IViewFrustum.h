#ifndef _YON_SCENE_CAMERA_IVIEWFRUSTUM_H_
#define _YON_SCENE_CAMERA_IVIEWFRUSTUM_H_

#include "plane3d.h"
#include "vector3d.h"
#include "line3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "IRenderable.h"

namespace yon{
namespace scene{
namespace camera{

	enum ENUM_FRUSTUM_PLANE{
		ENUM_FRUSTUM_PLANE_FAR = 0,
		ENUM_FRUSTUM_PLANE_NEAR,
		ENUM_FRUSTUM_PLANE_LEFT,
		ENUM_FRUSTUM_PLANE_RIGHT,
		ENUM_FRUSTUM_PLANE_TOP,
		ENUM_FRUSTUM_PLANE_BOTTOM,
		ENUM_FRUSTUM_PLANE_COUNT
	};

	class IViewFrustum : public core::IRenderable{
	public:
		//! This constructor creates a view frustum based on a projection and/or view matrix.
		virtual void setFrom(const core::matrix4f& mat)= 0;

		//! returns the point which is on the specific corner inside the the view frustum.
		virtual core::vector3df getFarLeftTop() const=0;
		virtual core::vector3df getFarLeftBottom() const=0;
		virtual core::vector3df getFarRightTop() const=0;
		virtual core::vector3df getFarRightBottom() const=0;
		virtual core::vector3df getNearLeftTop() const=0;
		virtual core::vector3df getNearLeftBottom() const=0;
		virtual core::vector3df getNearRightTop() const=0;
		virtual core::vector3df getNearRightBottom() const=0;

		virtual void setCameraPosition(const core::vector3df& pos) = 0;

		virtual bool intersectWithBox(const core::vector3df& pos, f32 size) = 0;

		//! recalculates the bounding box member based on the planes
		virtual  void recalculateBoundingBox() = 0;
		//! returns a bounding box enclosing the whole view frustum
		virtual const core::aabbox3df &getBoundingBox() const=0;

		virtual void render(video::IVideoDriver* driver)=0;
	};
}
}
}
#endif