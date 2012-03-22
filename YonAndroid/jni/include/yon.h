#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "config.h" 
#include "yonTypes.h" 

#include "color.h" 
#include "dimension2d.h" 
#include "dimension3d.h" 
#include "exception.h" 
#include "vector2d.h" 
#include "vector3d.h" 
#include "yonArray.h" 
#include "yonList.h"
#include "yonMath.h" 
#include "yonString.h" 
#include "matrix4.h" 
#include "path.h" 
#include "rect.h" 

#include "IEntity.h" 
#include "IFileSystem.h" 
#include "IGeometryFactory.h" 
#include "ILogger.h" 
#include "IModel.h" 
#include "IReferencable.h" 
#include "ISceneManager.h" 
#include "ITimer.h" 
#include "IUnit.h" 
#include "IVideoDriver.h" 
#include "IYonEngine.h" 

#include "SVertex.h" 
#include "SVideoDriverInfo.h" 
#include "SYonEngineParameters.h" 
#include "SYonPlatformInfo.h" 



namespace yon{

	extern "C" YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif