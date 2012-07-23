#include "CViewFrustum.h"
#include "IVideoDriver.h"

namespace yon{
namespace scene{
namespace camera{

	core::vector3df CViewFrustum::getFarLeftTop() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_TOP],
			m_planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	core::vector3df CViewFrustum::getFarLeftBottom() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			m_planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	core::vector3df CViewFrustum::getFarRightTop() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_TOP],
			m_planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	core::vector3df CViewFrustum::getFarRightBottom() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			m_planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	core::vector3df CViewFrustum::getNearLeftTop() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_TOP],
			m_planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	core::vector3df CViewFrustum::getNearLeftBottom() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			m_planes[ENUM_FRUSTUM_PLANE_LEFT], p);

		return p;
	}

	core::vector3df CViewFrustum::getNearRightTop() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_TOP],
			m_planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	core::vector3df CViewFrustum::getNearRightBottom() const
	{
		core::vector3df p;
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(
			m_planes[ENUM_FRUSTUM_PLANE_BOTTOM],
			m_planes[ENUM_FRUSTUM_PLANE_RIGHT], p);

		return p;
	}

	const core::aabbox3df &CViewFrustum::getBoundingBox() const
	{
		return m_boundingBox;
	}

	void CViewFrustum::recalculateBoundingBox()
	{
		m_boundingBox.reset ( m_cameraPosition );

		m_boundingBox.addInternalPoint(getFarLeftTop());
		m_boundingBox.addInternalPoint(getFarRightTop());
		m_boundingBox.addInternalPoint(getFarLeftBottom());
		m_boundingBox.addInternalPoint(getFarRightBottom());
	}

	void CViewFrustum::setFrom(const core::matrix4f& mat)
	{
		// left clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_LEFT].normal.x = mat.m[0][3] + mat.m[0][0];
		m_planes[ENUM_FRUSTUM_PLANE_LEFT].normal.y = mat.m[1][3] + mat.m[1][0];
		m_planes[ENUM_FRUSTUM_PLANE_LEFT].normal.z = mat.m[2][3] + mat.m[2][0];
		m_planes[ENUM_FRUSTUM_PLANE_LEFT].d =        mat.m[3][3] + mat.m[3][0];

		// right clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.x = mat.m[0][3] - mat.m[0][0];
		m_planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.y = mat.m[1][3] - mat.m[1][0];
		m_planes[ENUM_FRUSTUM_PLANE_RIGHT].normal.z = mat.m[2][3] - mat.m[2][0];
		m_planes[ENUM_FRUSTUM_PLANE_RIGHT].d =        mat.m[3][3] - mat.m[3][0];

		// top clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_TOP].normal.x = mat.m[0][3] - mat.m[0][1];
		m_planes[ENUM_FRUSTUM_PLANE_TOP].normal.y = mat.m[1][3] - mat.m[1][1];
		m_planes[ENUM_FRUSTUM_PLANE_TOP].normal.z = mat.m[2][3] - mat.m[2][1];
		m_planes[ENUM_FRUSTUM_PLANE_TOP].d =        mat.m[3][3] - mat.m[3][1];

		// bottom clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.x = mat.m[0][3] + mat.m[0][1];
		m_planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.y = mat.m[1][3] + mat.m[1][1];
		m_planes[ENUM_FRUSTUM_PLANE_BOTTOM].normal.z = mat.m[2][3] + mat.m[2][1];
		m_planes[ENUM_FRUSTUM_PLANE_BOTTOM].d =        mat.m[3][3] + mat.m[3][1];

		// far clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_FAR].normal.x = mat.m[0][3] - mat.m[0][2];
		m_planes[ENUM_FRUSTUM_PLANE_FAR].normal.y = mat.m[1][3] - mat.m[1][2];
		m_planes[ENUM_FRUSTUM_PLANE_FAR].normal.z = mat.m[2][3] - mat.m[2][2];
		m_planes[ENUM_FRUSTUM_PLANE_FAR].d =        mat.m[3][3] - mat.m[3][2];

		// near clipping plane
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].normal.x = mat.m[0][3] + mat.m[0][2];
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].normal.y = mat.m[1][3] + mat.m[1][2];
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].normal.z = mat.m[2][3] + mat.m[2][2];
		m_planes[ENUM_FRUSTUM_PLANE_NEAR].d =        mat.m[3][3] + mat.m[3][2];

		// normalize normals
		u32 i;
		for ( i=0; i != ENUM_FRUSTUM_PLANE_COUNT; ++i)
		{
			const f32 len = -core::reciprocal_squareroot(m_planes[i].normal.getLengthSQ());
			m_planes[i].normal *= len;
			m_planes[i].d *= len;
		}

		// make bounding box
		recalculateBoundingBox();
	}

	void CViewFrustum::render(video::IVideoDriver* driver)
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