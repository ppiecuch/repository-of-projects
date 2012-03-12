#ifndef _YON_IYONENGINE_H_
#define _YON_IYONENGINE_H_


#include "config.h"
#include "IReferencable.h"
#include "SYonPlatformInfo.h"
#include "ITimer.h"
#include "IVideoDriver.h"
#include "ILogger.h"


namespace yon{

	class IYonEngine:public virtual core::IReferencable{
	public:
		virtual ~IYonEngine(){};
		virtual video::IVideoDriver* getVideoDriver() = 0;
		virtual debug::ILogger* getLogger() = 0;
		virtual bool run() = 0;
		//更新
		//包括更新计时器
		//virtual void update() = 0;

		//TODO
		//virtual yon::ITimer* getTimer() = 0;
		//virtual bool getPlatformInfo(SYonPlatformInfo& info) = 0;
	protected:
		virtual void createDriver() = 0;
	};
}

#endif