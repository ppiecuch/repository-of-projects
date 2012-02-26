#ifndef _YON_CORE_DIMENSION_2D_H_
#define _YON_CORE_DIMENSION_2D_H_

#include "types.h"

namespace yon{
	namespace core{
		template <class T>
		class dimension2d{
		public:
			T w,h;

			dimension2d():w(0),h(0){}
			dimension2d(const T& width,const T& height):w(width),h(height){}

			//¼ÆËãÃæ»ý
			T getArea() const{
				return w*h;
			}
		};
		typedef dimension2d<f32> dimension2df;
		typedef dimension2d<u32> dimension2du;
		typedef dimension2d<s32> dimension2di;
	}
}
#endif