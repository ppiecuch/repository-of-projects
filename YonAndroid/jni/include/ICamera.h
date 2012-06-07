#ifndef _YON_SCENE_CAMERA_ICAMERA_H_
#define _YON_SCENE_CAMERA_ICAMERA_H_

#include "IRenderable.h"
#include "plane.h"
#include "matrix4.h"
#include "dimension2d.h"

namespace yon{
	namespace scene{
		namespace camera{

			enum ENUM_CAMERA_TYPE{
				ENUM_CAMERA_TYPE_PERSP = 0,
				ENUM_CAMERA_TYPE_ORTHO,
				ENUM_CAMERA_TYPE_COUNT
			};

			enum ENUM_FRUSTUM_PLANE{
				ENUM_FRUSTUM_PLANE_FAR = 0,
				ENUM_FRUSTUM_PLANE_NEAR,
				ENUM_FRUSTUM_PLANE_LEFT,
				ENUM_FRUSTUM_PLANE_RIGHT,
				ENUM_FRUSTUM_PLANE_TOP,
				ENUM_FRUSTUM_PLANE_BOTTOM,
				ENUM_FRUSTUM_PLANE_COUNT
			};
		
			class ICamera : public core::IRenderable{
			protected:
				enum ENUM_FRUSTUM_TRANSFORM{
					ENUM_FRUSTUM_TRANSFORM_VIEW = 0,		//视图空间矩阵
					ENUM_FRUSTUM_TRANSFORM_PROJECTION,		//投影空间矩阵
					ENUM_FRUSTUM_TRANSFORM_COUNT
				};
				core::planef m_planes[ENUM_FRUSTUM_PLANE_COUNT];
				bool m_bVisible;
				core::matrix4f m_matrixs[ENUM_FRUSTUM_TRANSFORM_COUNT];
				core::vector3df m_target;
				core::vector3df m_up;		//保持归一化
				core::vector3df m_direction;//保持归一化
				f32 m_fNear;
				f32 m_fFar;
				bool m_bNeedUpload;
			public:
				ICamera(const core::vector3df& pos=core::vector3df(0,0,1),
					const core::vector3df& up=core::vector3df(0,1,0),
					const core::vector3df& lookat = core::vector3df(0,0,-1),bool visible=false):
					IRenderable(pos),m_up(up),m_target(lookat),m_direction(lookat-pos),m_bVisible(visible),m_fNear(1),m_fFar(3000.0f),m_bNeedUpload(true){
						m_up.normalize();
						m_direction.normalize();
						//for(u32 i=0;i<ENUM_FRUSTUM_TRANSFORM_COUNT;++i)
						//	m_matrixs[i].makeIdentity();
					}

				void setNeedUpload(){
						m_bNeedUpload=true;
				}

				virtual void onResize(const core::dimension2du& size) = 0;

				virtual void setUpVector(const core::vector3df& up){
					m_up=up;
				}
				virtual const core::vector3df& getUpVector() const{
					return m_up;
				}

				virtual f32 getNear() const{
					return m_fNear;
				}
				virtual void setNear(f32 n){
					m_fNear=n;
				}

				virtual f32 getFar() const{
					return m_fFar;
				}
				virtual void setFar(f32 f){
					m_fFar=f;
				}

				virtual void setVisible(bool on){
					m_bVisible=on;
				}
				virtual bool isVisible() const{
					return m_bVisible;
				}

				virtual void setPosition(const core::vector3df& pos){
					IRenderable::setPosition(pos);
					m_direction=m_target-pos;
					m_direction.normalize();
				}

				//TODO setDirection
				inline const core::vector3df& getDirection() const{
					return m_direction;
				}

				inline const core::planef& getPlane(ENUM_FRUSTUM_PLANE which) const{
					return m_planes[which];
				}
				inline const core::matrix4f& getViewTransform() const{
					return m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW];
				}
				inline const core::matrix4f& getProjectionTransform() const{
					return m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION];
				}

				virtual ENUM_CAMERA_TYPE getType() const = 0;
			};
		}
	}
}
#endif