#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "config.h"

#include "YonEngine.h"
#include "YonEngineParameters.h"

namespace yon{

	extern "C" YON_API YonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif