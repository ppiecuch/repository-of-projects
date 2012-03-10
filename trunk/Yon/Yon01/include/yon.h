#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "config.h"

#include "color.h" 
#include "dimension2d.h" 
#include "IReferencable.h" 
#include "matrix4.h"
#include "rect.h" 
#include "vector3d.h"
#include "yonTypes.h" 
#include "yonString.h" 
#include "ITimer.h" 
#include "SYonEngineParameters.h" 
#include "yon.h" 
#include "IYonEngine.h" 

namespace yon{

	extern "C" YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif