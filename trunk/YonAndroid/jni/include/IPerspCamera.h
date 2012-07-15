#ifndef _YON_SCENE_CAMERA_IPERSPCAMERA_H_
#define _YON_SCENE_CAMERA_IPERSPCAMERA_H_

#include "ICamera.h"
#include "yonMath.h"

namespace yon{
namespace scene{
namespace camera{

	class IPerspCamera : public ICamera{
	protected:
		f32 m_fFovy;
		f32 m_fAspectRatio;
	public:
		IPerspCamera(IModel* parent=NULL,
			const core::vector3df& pos = core::vector3df(0,0,1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,-1)):
			ICamera(parent,pos,up,lookat),m_fFovy(core::PI2/5),m_fAspectRatio(1)
		{}
		virtual f32 getAspectRatio() const{
			return m_fAspectRatio;
		}
		virtual void setAspectRatio(f32 aspect){
			m_fAspectRatio=aspect;
		}

		virtual f32 getFovy() const{
			return m_fFovy;
		}
		virtual void setFovy(f32 fovy){
			m_fFovy=fovy;
		}

		virtual ENUM_CAMERA_TYPE getType() const{
			return ENUM_CAMERA_TYPE_PERSP;
		}

	};
}//camera
}//scene
}//yon
#endif