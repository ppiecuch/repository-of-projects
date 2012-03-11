#include "yon.h"

#include "CYonEngineWin32.h"


namespace yon{

	namespace core
	{
		const matrix4f IDENTITY_MATRIX(true);
	}


	YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param){

		printf("CreateEngine(%d,%d)\n",param.windowSize.w,param.windowSize.h);

		IYonEngine* engine;

#ifdef YON_VIDEO_MODE_OGLES1
		engine=new platform::CYonEngineWin32(param);
#endif

		YON_DEBUG_BREAK_IF(engine==NULL)

		return engine;
	}
}