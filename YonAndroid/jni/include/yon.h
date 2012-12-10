#ifndef _YON_YON_H_
#define _YON_YON_H_

#include "yonConfig.h"
#include "yonTypes.h" 
#include "heapsort.h"
#ifdef YON_COMPILE_WITH_WIN32
#include "btree.h"
#include "bstree.h"
#include "rbtree.h"
#endif

#include "ISound.h"
#include "IWave.h"
#include "IWaveLoader.h"
#include "IAudioDriver.h"
#include "IListener.h"

#include "ILogger.h" 
#include "IProfile.h"

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
#include "vector2d.h" 
#include "vector3d.h" 
#include "aabbox3d.h"
#include "yonArray.h" 
#include "yonList.h"
#include "yonLoop.h"
#include "yonMath.h" 
#include "yonMap.h"
#include "yonString.h" 
#include "matrix4.h" 
#include "rect.h" 
#include "IReferencable.h" 
#include "yonUtil.h"

#include "path.h" 
#include "IFileSystem.h" 
#include "IStream.h"

#include "II18NManager.h"

#include "INetManager.h"

#include "ICallback.h"

#include "SColor.h"
#include "SVertex.h" 
#include "IImage.h"
#include "IImageLoader.h"
#include "ITexture.h"
#include "SMaterialLayer.h"
//#include "IMaterial.h"
#include "SMaterial.h"
#include "IMaterialRenderer.h"
#include "IEntity.h" 
#include "IAnimatedEntity.h"
#include "IGeometryFactory.h" 
#include "ISceneNode.h" 
#include "ITerrainModel.h"
#include "IWaterModel.h"
#include "IAnimatedSceneNode.h"
#include "IViewFrustum.h"
#include "ICamera.h"
#include "ISceneManager.h" 
#include "ITimer.h" 
#include "IUnit.h" 
#include "SShap.h"
#include "IVideoDriver.h" 
#include "IRandomizer.h"
#include "IYonEngine.h" 

#include "SVideoDriverInfo.h" 
#include "SYonEngineParameters.h" 
#include "SYonPlatformInfo.h" 



namespace yon{

	extern "C" YON_API IYonEngine* CreateEngine(const SYonEngineParameters& param);
}

#endif