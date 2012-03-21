#ifndef _YON_SCENE_CAMERA_CCAMERA_H_
#define _YON_SCENE_CAMERA_CCAMERA_H_

#include "ICamera.h"
#include "SViewFrustum.h"

namespace yon{
	namespace scene{
		namespace camera{

			class CCamera : public ICamera{
			private:
				SViewFrustum m_viewFrustum;
			public:

				virtual void render(video::IVideoDriver* driver);

				virtual void setPosition(const core::vector3df& pos){
					m_viewFrustum.setPosition(pos);
				}
				virtual const core::vector3df& getPosition() const{
					return m_viewFrustum.getPosition();
				}

				virtual void setRotation(const core::vector3df& rotation){
					//TODO
					ICamera::setRotation(rotation);
				}
				virtual const core::vector3df& getRotation(){
					//TODO
					return ICamera::getRotation();
				}
			};
		}
	}
}
#endif