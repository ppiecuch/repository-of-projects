#ifndef _YON_PLATFORM_SYONPLATFORMINFO_H_
#define _YON_PLATFORM_SYONPLATFORMINFO_H_

#include "yonString.h"

namespace yon{
	namespace platform{

		enum ENUM_PLATFORM_TYPE{
			ENUM_PLATFORM_TYPE_WINDOW = 0,
			ENUM_PLATFORM_TYPE_ANDROID,
			ENUM_PLATFORM_TYPE_COUNT
		};

		struct SYonPlatformInfo{

			ENUM_PLATFORM_TYPE pfType;
			core::stringc osName;
			core::stringc osVersion;
			core::stringc vendor;

#ifdef YON_COMPILE_WITH_ANDROID
			core::stringc jniVersion;
#endif
		};
	}
}
#endif