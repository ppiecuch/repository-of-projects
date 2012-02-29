#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "config.h"

#include "color.h" 
#include "dimension2d.h" 
#include "IReferencable.h" 
#include "ITimer.h" 
#include "rect.h" 
#include "yonString.h" 
#include "SYonEngineParameters.h" 
#include "yonTypes.h" 
#include "yon.h" 
#include "YonEngine.h" 

namespace yon{

	extern "C" YON_API YonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif