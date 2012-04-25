#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "config.h" 
#include "yonTypes.h" 
#include "heapsort.h"
#include "btree.h"
#include "bstree.h"
#include "rbtree.h"

#include "ISound.h"
#include "IWave.h"
#include "IWaveLoader.h"
#include "IAudioDriver.h"
#include "IListener.h"

#include "ILogger.h" 
#include "IDebugPrinter.h"

#include "dimension2d.h" 
#include "dimension3d.h" 
#include "exception.h" 
#include "plane.h"
#include "position2d.h"
#include "position3d.h"
#include "vector2d.h" 
#include "vector3d.h" 
#include "yonArray.h" 
#include "yonList.h"
#include "yonMath.h" 
#include "yonMap.h"
#include "yonString.h" 
#include "matrix4.h" 
#include "rect.h" 
#include "IReferencable.h" 

#include "path.h" 
#include "IFileSystem.h" 
#include "IReadFile.h"
#include "IStream.h"


#include "SColor.h"
#include "SVertex.h" 
#include "IImage.h"
#include "IImageLoader.h"
#include "ITexture.h"
#include "SMaterialLayer.h"
#include "IMaterial.h"
#include "IMaterialRenderer.h"
#include "IEntity.h" 
#include "IGeometryFactory.h" 
#include "IModel.h" 
#include "ICamera.h"
#include "ISceneManager.h" 
#include "ITimer.h" 
#include "IUnit.h" 
#include "SShap.h"
#include "IVideoDriver.h" 
#include "IYonEngine.h" 

#include "SVideoDriverInfo.h" 
#include "SYonEngineParameters.h" 
#include "SYonPlatformInfo.h" 



namespace yon{

	extern "C" YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif