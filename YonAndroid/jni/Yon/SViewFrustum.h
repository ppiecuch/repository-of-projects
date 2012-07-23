#ifndef _YON_SCENE_CAMERA_SVIEWFRUSTUM_H_
#define _YON_SCENE_CAMERA_SVIEWFRUSTUM_H_
#if 0
#include "plane3d.h"
#include "vector3d.h"
#include "line3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "IVideoDriver.h"
#include "IRenderable.h"

namespace yon{
namespace scene{
namespace camera{

	struct SViewFrustum : public IRenderable{
		enum ENUM_FRUSTUM_PLANE
		{
			ENUM_FRUSTUM_PLANE_FAR = 0,
			ENUM_FRUSTUM_PLANE_NEAR,
			ENUM_FRUSTUM_PLANE_LEFT,
			ENUM_FRUSTUM_PLANE_RIGHT,
			ENUM_FRUSTUM_PLANE_BOTTOM,
			ENUM_FRUSTUM_PLANE_TOP,

			//! Amount of planes enclosing the view frustum. Should be 6.
			ENUM_FRUSTUM_PLANE_COUNT
		};

		//! the position of the camera
		core::vector3df cameraPosition;
		//! all planes enclosing the view frustum.
		core::plane3df planes[ENUM_FRUSTUM_PLANE_COUNT];
		//! bounding box around the view frustum
		core::aabbox3df boundingBox;

		SViewFrustum() {}
		SViewFrustum(const SViewFrustum& other);

		//! This constructor creates a view frustum based on a projection and/or view matrix.
		inline void setFrom(const core::matrix4f& mat);

		//! returns the point which is on the specific corner inside the the view frustum.
		core::vector3df getFarLeftTop() const;
		core::vector3df getFarLeftBottom() const;
		core::vector3df getFarRightTop() const;
		core::vector3df getFarRightBottom() const;
		core::vector3df getNearLeftTop() const;
		core::vector3df getNearLeftBottom() const;
		core::vector3df getNearRightTop() const;
		core::vector3df getNearRightBottom() const;

		//! recalculates the bounding box member based on the planes
		inline void recalculateBoundingBox();
		//! returns a bounding box enclosing the whole view frustum
		const core::aabbox3df &getBoundingBox() const;

		virtual void render(video::IVideoDriver* driver);
	};

	inline core::vector3df SViewFrustum::getFarLeftTop() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_TOP],
			planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getFarLeftBottom() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getFarRightTop() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_TOP],
			planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getFarRightBottom() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getNearLeftTop() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_TOP],
			planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getNearLeftBottom() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getNearRightTop() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_TOP],
			planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	inline core::vector3df SViewFrustum::getNearRightBottom() const
	{
		core::vector3df p;
		planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	inline const core::aabbox3df &SViewFrustum::getBoundingBox() const
	{
		return boundingBox;
	}

	inline void SViewFrustum::recalculateBoundingBox()
	{
		boundingBox.reset ( cameraPosition );

		boundingBox.addInternalPoint(getFarLeftTop());
		boundingBox.addInternalPoint(getFarRightTop());
		boundingBox.addInternalPoint(getFarLeftBottom());
		boundingBox.addInternalPoint(getFarRightBottom());
	}

	//! This constructor creates a view frustum based on a projection
	//! and/or view matrix.
	inline void SViewFrustum::setFrom(const core::matrix4f& mat)
	{
		// left clipping plane
		planes[ENUM_FRUSTUM_PLANE_LEFT].normal.x = mat.m[0][3] + mat.m[0][0];
		planes[ENUM_FRUSTUM_PLANE_LEFT].normal.y = mat.m[1][3] + mat.m[1][0];
		planes[ENUM_FRUSTUM_PLANE_LEFT].normal.z = mat.m[2][3] + mat.m[2][0];
		planes[ENUM_FRUSTUM_PLANE_LEFT].d =        mat.m[3][3] + mat.m[3][0];

		// right clipping plane
		planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.x = mat.m[0][3] - mat.m[0][0];
		planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.y = mat.m[1][3] - mat.m[1][0];
		planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.z = mat.m[2][3] - mat.m[2][0];
		planes[ENUM_FRUSTUM_PLANE_RIGHT].d =        mat.m[3][3] - mat.m[3][0];

		// top clipping plane
		planes[ENUM_FRUSTUM_PLANE_TOP].normal.x = mat.m[0][3] - mat.m[0][1];
		planes[ENUM_FRUSTUM_PLANE_TOP].normal.y = mat.m[1][3] - mat.m[1][1];
		planes[ENUM_FRUSTUM_PLANE_TOP].normal.z = mat.m[2][3] - mat.m[2][1];
		planes[ENUM_FRUSTUM_PLANE_TOP].d =        mat.m[3][3] - mat.m[3][1];

		// bottom clipping plane
		planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.x = mat.m[0][3] + mat.m[0][1];
		planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.y = mat.m[1][3] + mat.m[1][1];
		planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.z = mat.m[2][3] + mat.m[2][1];
		planes[ENUM_FRUSTUM_PLANE_BOTTOM].d =        mat.m[3][3] + mat.m[3][1];

		// far clipping plane
		planes[ENUM_FRUSTUM_PLANE_FAR].normal.x = mat.m[0][3] - mat.m[0][2];
		planes[ENUM_FRUSTUM_PLANE_FAR].normal.y = mat.m[1][3] - mat.m[1][2];
		planes[ENUM_FRUSTUM_PLANE_FAR].normal.z = mat.m[2][3] - mat.m[2][2];
		planes[ENUM_FRUSTUM_PLANE_FAR].d =        mat.m[3][3] - mat.m[3][2];

		// near clipping plane
		planes[ENUM_FRUSTUM_PLANE_NEAR].normal.x = mat.m[0][3] + mat.m[0][2];
		planes[ENUM_FRUSTUM_PLANE_NEAR].normal.y = mat.m[1][3] + mat.m[1][2];
		planes[ENUM_FRUSTUM_PLANE_NEAR].normal.z = mat.m[2][3] + mat.m[2][2];
		planes[ENUM_FRUSTUM_PLANE_NEAR].d =        mat.m[3][3] + mat.m[3][2];

		// normalize normals
		u32 i;
		for ( i=0; i != ENUM_FRUSTUM_PLANE_COUNT; ++i)
		{
			const f32 len = -core::reciprocal_squareroot(planes[i].normal.getLengthSQ());
			planes[i].normal *= len;
			planes[i].d *= len;
		}

		// make bounding box
		recalculateBoundingBox();
	}

	inline void SViewFrustum::render(video::IVideoDriver* driver)
	{
		driver->setMaterial(video::DEFAULT_MATERIAL);
		driver->setTransform(video::ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
		core::vector3df flt,frt,flb,frb,nlt,nrt,nlb,nrb;
		flt=getFarLeftTop();
		frt=getFarRightTop();
		flb=getFarLeftBottom();
		frb=getFarRightBottom();
		nlt=getNearLeftTop();
		nrt=getNearRightTop();
		nlb=getNearLeftBottom();
		nrb=getNearRightBottom();

		driver->draw3DLine(nlt,nrt);
		driver->draw3DLine(nlb,nrb);
		driver->draw3DLine(nlt,nlb);
		driver->draw3DLine(nrt,nrb);

		driver->draw3DLine(nlt,flt);
		driver->draw3DLine(nlb,flb);
		driver->draw3DLine(nrt,frt);
		driver->draw3DLine(nrb,frb);

		driver->draw3DLine(flt,frt);
		driver->draw3DLine(flb,frb);
		driver->draw3DLine(flt,flb);
		driver->draw3DLine(frt,frb);
	}
}
}
}
#endif
#endif