#ifndef _YON_SCENE_CAMERA_SVIEWFRUSTUM_H_
#define _YON_SCENE_CAMERA_SVIEWFRUSTUM_H_

#include "plane.h"
#include "matrix4.h"

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

			class SViewFrustum{
			private:
				enum ENUM_FRUSTUM_TRANSFORM{
					ENUM_FRUSTUM_TRANSFORM_VIEW = 0,		//视图空间矩阵
					ENUM_FRUSTUM_TRANSFORM_PROJECTION,		//投影空间矩阵
					ENUM_FRUSTUM_TRANSFORM_COUNT
				};
				core::matrix4f m_matrixs[ENUM_FRUSTUM_TRANSFORM_COUNT];
			public:

				SViewFrustum() {}
				SViewFrustum(const SViewFrustum& other);

				void transform(const core::matrix4f& mat);


				inline const core::matrix4f& SViewFrustum::getViewTransform() const{
					return m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW];
				}
				inline const core::matrix4f& SViewFrustum::getProjectionTransform() const{
					return m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION];
				}


				core::planef m_planes[ENUM_FRUSTUM_PLANE_COUNT];
				
			};

			inline SViewFrustum::SViewFrustum(const SViewFrustum& other)
			{
				u32 i;
				for (i=0; i<ENUM_FRUSTUM_PLANE_COUNT; ++i)
					m_planes[i]=other.m_planes[i];

				for (i=0; i<ENUM_FRUSTUM_TRANSFORM_COUNT; ++i)
					m_matrixs[i]=other.m_matrixs[i];
			}

			inline void SViewFrustum::transform(const core::matrix4f& mat)
			{
				//for (u32 i=0; i<ENUM_FRUSTUM_PLANE_COUNT; ++i)
				//	mat.transformPlane(m_planes[i]);
				//TODO transform view matrix
			}
		}//camera
	}//scene
}//yon
#endif