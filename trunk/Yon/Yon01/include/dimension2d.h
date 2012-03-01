#ifndef _YON_CORE_DIMENSION_2D_H_
#define _YON_CORE_DIMENSION_2D_H_

#include "yonTypes.h"

namespace yon{
	namespace core{
		template <class T>
		class dimension2d{
		public:
			T w,h;

			dimension2d():w(0),h(0){}
			dimension2d(const T& width,const T& height):w(width),h(height){}
			dimension2d(const dimension2d<T>& d):w(d.w),h(d.h){}

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