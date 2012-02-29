#ifndef _YON_PLATFORM_IYONPLATFORM_H_
#define _YON_PLATFORM_IYONPLATFORM_H_

#include "IReferencable.h"
#include "SYonPlatformInfo.h"
#include "ITimer.h"

//负责平台相关的一些处理
namespace yon{
	namespace platform{

		class IYonPlatform:public virtual yon::core::IReferencable{
		public:
			//TODO
			//virtual yon::ITimer* getTimer() = 0;
			//virtual bool getPlatformInfo(SYonPlatformInfo& info) = 0;

		};
	}
}
#endif