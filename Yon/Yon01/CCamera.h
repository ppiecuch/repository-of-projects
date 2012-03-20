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
			};
		}
	}
}
#endif