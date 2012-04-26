#ifndef _YON_CORE_POSITION3D_H_
#define _YON_CORE_POSITION3D_H_

#include "yonConfig.h"
#include "vector3d.h"

#define position3d vector3d
namespace yon{
	namespace core{

		typedef position3d<f32> position3df;
		typedef position3d<s32> position3di;

		YON_API extern const position3df ORIGIN_POSITION3DF;
		YON_API extern const position3di ORIGIN_POSITION3DI;
	}
}

#endif