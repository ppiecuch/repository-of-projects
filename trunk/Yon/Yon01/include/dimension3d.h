#ifndef _YON_CORE_DIMENSION3D_H_
#define _YON_CORE_DIMENSION3D_H_

#include "yonTypes.h"

namespace yon{
	namespace core{

		template <class T>
		class dimension3d{
		public:
			T w,h,d;

			dimension3d():w(0),h(0),d(0){}
			dimension3d(const T& width,const T& height,const T& depth):w(width),h(height),d(depth){}
			dimension3d(const dimension3d<T>& d):w(d.w),h(d.h),d(d.d){}

			//¼ÆËãÌå»ý
			T getVolumn() const{
				return w*h*d;
			}
		};
		typedef dimension3d<f32> dimension3df;
		typedef dimension3d<u32> dimension3du;
		typedef dimension3d<s32> dimension3di;
	}
}
#endif