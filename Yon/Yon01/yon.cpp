#include "yon.h"


namespace yon{

	YON_API YonEngine* CreateEngine(const SYonEngineParameters& param){

		printf("CreateEngine(%d,%d)\n",param.windowSize.w,param.windowSize.h);

#ifdef YON_VIDEO_MODE_OGLES1
#endif


		return NULL;
	}
}