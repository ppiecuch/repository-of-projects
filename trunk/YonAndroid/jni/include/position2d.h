#ifndef _YON_CORE_POSITION2D_H_
#define _YON_CORE_POSITION2D_H_

#include "config.h"
#include "vector2d.h"

#define position2d vector2d
namespace yon{
	namespace core{

		typedef position2d<f32> position2df;
		typedef position2d<s32> position2di;

		YON_API extern const position2df ORIGIN_POSITION2DF;
		YON_API extern const position2di ORIGIN_POSITION2DI;
	}
}

#endif