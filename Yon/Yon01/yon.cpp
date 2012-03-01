#include "yon.h"

#include "CYonEngineWin32.h"


namespace yon{

	YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param){

		printf("CreateEngine(%d,%d)\n",param.windowSize.w,param.windowSize.h);

#ifdef YON_VIDEO_MODE_OGLES1
		platform::CYonEngineWin32* engine=new platform::CYonEngineWin32(param);
#endif


		return NULL;
	}
}