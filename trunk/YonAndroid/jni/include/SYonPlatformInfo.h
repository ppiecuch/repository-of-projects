#ifndef _YON_PLATFORM_SYONPLATFORMINFO_H_
#define _YON_PLATFORM_SYONPLATFORMINFO_H_

#include "yonString.h"

namespace yon{
	namespace platform{

		struct SYonPlatformInfo{
			yon::core::stringc osName;
			yon::core::stringc osVersion;
			yon::core::stringc vendor;
		};
	}
}
#endif