#include "yon.h"
#include "CLogger.h"
#include "CYonEngineWin32.h"

namespace yon{

	

	namespace core
	{
		const matrix4f IDENTITY_MATRIX(true);
	}

	namespace debug{
		ILogger* Logger=NULL;
	}

	using namespace debug;

	YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param){

		if(Logger==NULL){
			Logger=new debug::CLogger();
		}else{
			Logger->grab();
		}

		IYonEngine* engine;

#ifdef YON_VIDEO_MODE_OGLES1
		engine=new platform::CYonEngineWin32(param);
#endif

		YON_DEBUG_BREAK_IF(engine==NULL)

		Logger->info(YON_LOG_SUCCEED_FORMAT,core::stringc("CreateEngine(%d,%d)",param.windowSize.w,param.windowSize.h).c_str());

		return engine;
	}
}