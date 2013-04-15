#ifndef _YON_CORE_H_
#define _YON_CORE_H_

#include "yonConfig.h"
#include "yonTypes.h" 
#include "heapsort.h"

#include "ILogger.h" 

#include "delegate.h"
#include "dimension2d.h" 
#include "dimension3d.h" 
#include "exception.h" 
#include "fastatof.h"
#include "line2d.h"
#include "line3d.h"
#include "objectpool.h"
#include "plane3d.h"
#include "position2d.h"
#include "position3d.h"
#include "quaternion.h"
#include "randomizer.h"
#include "vector2d.h" 
#include "vector3d.h" 
#include "yonArray.h" 
#include "yonList.h"
#include "yonLoop.h"
#include "yonMath.h" 
#include "yonMap.h"
#include "yonMutex.h"
#include "yonSemaphore.h"
#include "yonString.h" 
#include "yonUString.h"
#include "matrix4.h" 
#include "rect.h" 
#include "IReferencable.h" 
#include "yonUtil.h"

#include "IThread.h"
#include "IHook.h"

#include "path.h" 
#include "IFileSystem.h" 
#include "IStream.h"

namespace yon{

	extern "C" YON_API void initYon();
	extern "C" YON_API void destroyYon();
}
#endif