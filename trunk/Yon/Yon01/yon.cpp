#include "yon.h"


namespace yon{

	YON_API YonEngine* CreateEngine(const SYonEngineParameters& param){

		printf("CreateEngine(%d,%d)\n",param.windowSize.w,param.windowSize.h);

#ifdef YON_COMPILE_WITH_WIN32
#endif

#ifdef YOU_COMPILE_WITH_ANDROID
#endif

#ifdef YOU_COMPILE_WITH_MAXOSX
#endif

		return NULL;
	}
}