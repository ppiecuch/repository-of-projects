#ifndef _YON_CORE_DIMENSION_2D_H_
#define _YON_CORE_DIMENSION_2D_H_

#include "yonTypes.h"
#include "yonMath.h"
#include "vector2d.h"

namespace yon{
	namespace core{
		template <class T>
		class dimension2d{
		public:
			T w,h;

			dimension2d():w(0),h(0){}
			dimension2d(const T& width,const T& height):w(width),h(height){}
			dimension2d(const dimension2d<T>& d):w(d.w),h(d.h){}

			template <class U>
			dimension2d<T>& operator=(const dimension2d<U>& other)
			{ 
				w = (T) other.w;
				h = (T) other.h;
				return *this;
			}


			//! Equality operator
			bool operator==(const dimension2d<T>& other) const
			{
				return core::equals(w, other.w) &&
					core::equals(h, other.h);
			}

			//! Inequality operator
			bool operator!=(const dimension2d<T>& other) const
			{
				return ! (*this == other);
			}

			bool operator==(const vector2d<T>& other) const;  // Defined in vector2d.h

			bool operator!=(const vector2d<T>& other) const
			{
				return !(*this == other);
			}

			//! Set to new values
			dimension2d<T>& set(const T& width, const T& height)
			{
				w = width;
				h = height;
				return *this;
			}

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