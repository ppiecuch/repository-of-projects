#ifndef _YON_VIDEO_OGLES1_COGLES1EXTENSIONHANDLER_H_
#define _YON_VIDEO_OGLES1_COGLES1EXTENSIONHANDLER_H_

#include "IVideoDriver.h"
#include "yonArray.h"

namespace yon{
namespace video{
namespace ogles1{

	class COGLES1ExtensionHandler{
	private:
		core::stringc m_extensions;
		bool m_featureAvailables[ENUM_VIDEO_FEATURE_COUNT];
	public:
		COGLES1ExtensionHandler();
		void initExtensionHandler();
		bool isFeatureAvailable(ENUM_VIDEO_FEATURE feature) const{
			return m_featureAvailables[feature];
		}
	};
}
}
}
#endif