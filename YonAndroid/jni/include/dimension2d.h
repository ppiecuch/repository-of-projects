#ifndef _YON_CORE_DIMENSION_2D_H_
#define _YON_CORE_DIMENSION_2D_H_

#include "yonTypes.h"
#include "yonMath.h"

namespace yon{
	namespace core{

		template <class T>
		class vector2d;

		template <class T>
		class dimension2d{
		public:
			T w,h;

			dimension2d():w(0),h(0){}
			dimension2d(const T& width,const T& height):w(width),h(height){}
			dimension2d(const dimension2d<T>& d):w(d.w),h(d.h){}
			dimension2d(const vector2d<T>& other); // Defined in vector2d.h

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

			//! Get the optimal size according to some properties
			/** This is a function often used for texture dimension
			calculations. The function returns the next larger or
			smaller dimension which is a power-of-two dimension
			(2^n,2^m) and/or square (Width=Height).
			\param requirePowerOfTwo Forces the result to use only
			powers of two as values.
			\param requireSquare Makes width==height in the result
			\param larger Choose whether the result is larger or
			smaller than the current dimension. If one dimension
			need not be changed it is kept with any value of larger.
			\param maxValue Maximum texturesize. if value > 0 size is
			clamped to maxValue
			\return The optimal dimension under the given
			constraints. */
			dimension2d<T> getOptimalSize(
					bool requirePowerOfTwo=true,
					bool requireSquare=false,
					bool larger=true,
					u32 maxValue = 0) const
			{
				u32 i=1;
				u32 j=1;
				if (requirePowerOfTwo)
				{
					while (i<(u32)w)
						i<<=1;
					if (!larger && i!=1 && i!=(u32)w)
						i>>=1;
					while (j<(u32)h)
						j<<=1;
					if (!larger && j!=1 && j!=(u32)h)
						j>>=1;
				}
				else
				{
					i=(u32)w;
					j=(u32)h;
				}

				if (requireSquare)
				{
					if ((larger && (i>j)) || (!larger && (i<j)))
						j=i;
					else
						i=j;
				}

				if ( maxValue > 0 && i > maxValue)
					i = maxValue;

				if ( maxValue > 0 && j > maxValue)
					j = maxValue;

				return dimension2d<T>((T)i,(T)j);
			}
		};
		typedef dimension2d<f32> dimension2df;
		typedef dimension2d<u32> dimension2du;
		typedef dimension2d<s32> dimension2di;

		YON_API extern const dimension2du ZERO_DIMENSION2DF;
		YON_API extern const dimension2du ZERO_DIMENSION2DU;
		YON_API extern const dimension2du ZERO_DIMENSION2DI;
	}
}
#endif