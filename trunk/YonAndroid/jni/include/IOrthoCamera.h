#ifndef _YON_SCENE_CAMERA_IORTHOCAMERA_H_
#define _YON_SCENE_CAMERA_IORTHOCAMERA_H_

#include "ICamera.h"

namespace yon{
namespace scene{
namespace camera{

	class IOrthoCamera : public ICamera{
	protected:
		f32 m_fLeft,m_fRight,m_fTop,m_fBottom;
	public:
		IOrthoCamera(
			const core::vector3df& pos = core::vector3df(0,0,-1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,100)):
			ICamera(pos,up,lookat),
			m_fLeft(-1),m_fRight(1),m_fTop(1),m_fBottom(-1)
			{}
		//virtual void setLeft(f32 left) = 0;
		virtual f32 getLeft() const{
			return m_fLeft;
		}
		//virtual void setRight(f32 right) = 0;
		virtual f32 getRight() const{
			return m_fRight;
		}
		//virtual void setTop(f32 top) = 0;
		virtual f32 getTop() const{
			return m_fTop;
		}
		//virtual void setBottom(f32 bottom) = 0;
		virtual f32 getBottom() const{
			return m_fBottom;
		}


		virtual ENUM_CAMERA_TYPE getType() const{
			return ENUM_CAMERA_TYPE_ORTHO;
		}
	};
}//camera
}//scene
}//yon
#endif