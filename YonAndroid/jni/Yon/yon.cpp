#include "yon.h"
#include "CLogger.h"
#include "CYonEngineWin32.h"
#include "CYonEngineAndroid.h"

namespace yon{

	

	namespace core
	{
		const matrix4f IDENTITY_MATRIX(true);

		const vector3df UP_VECTOR3DF(0,1,0);

		const color COLOR_GRAY(0x88888888);
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

#ifdef YON_COMPILE_WITH_WIN32
		engine=new platform::CYonEngineWin32(param);
#elif defined(YON_COMPILE_WITH_ANDROID)
		engine=new platform::CYonEngineAndroid(param);
#endif

		YON_DEBUG_BREAK_IF(engine==NULL)

			Logger->info(YON_LOG_SUCCEED_FORMAT,core::stringc("CreateEngine(%d,%d),version:%d.%d",param.windowSize.w,param.windowSize.h,YON_VERSION_MAJOR,YON_VERSION_MINOR).c_str());

		return engine;
	}
}