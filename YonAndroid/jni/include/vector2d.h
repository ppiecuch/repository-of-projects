#ifndef _YON_CORE_VECTOR2D_H_
#define _YON_CORE_VECTOR2D_H_

#include "yonConfig.h"
#include "yonTypes.h"
#include "yonMath.h"

namespace yon{
	namespace core{

		template<class T>
		class vector2d{
		public:
			T x,y;
			vector2d():x(0),y(0){}
			vector2d(T x,T y):x(x),y(y){}
			vector2d(const vector2d<T>& other) : x(other.x), y(other.y){}

			inline void print(){
				printf("%.3f,%.3f\n",x,y);
			}

			vector2d<T>& set(const T nx, const T ny) {x=nx; y=ny; return *this;}
			vector2d<T>& set(const vector2d<T>& p) {x=p.x; y=p.y; return *this;}

			vector2d<T>& rotateBy(f32 degrees, const vector2d<T>& center=vector2d<T>())
			{
				degrees *= DEGTORAD;
				const f32 cs = cos(degrees);
				const f32 sn = sin(degrees);

				x -= center.x;
				y -= center.y;

				set((T)(x*cs - y*sn), (T)(x*sn + y*cs));

				x += center.x;
				y += center.y;
				return *this;
			}

			//----->x
			//|
			//|
			//y
			//·¶Î§£º[0,360)
			inline f32 getAngle() const
			{
				if (y == 0) // corrected thanks to a suggestion by Jox
					return x < 0 ? 180 : 0;
				else if (x == 0)
					return y < 0 ? 270 : 90;

				// don't use getLength here to avoid precision loss with s32 vectors
				f32 tmp = y / sqrt((f32)(x*x + y*y));
				tmp = atan( core::squareroot(1 - tmp*tmp) / tmp) * RADTODEG;

				if (x>0 && y>0)
					return 90-tmp;
				else if (x>0 && y<0)
					return 270 - tmp;
				else if (x<0 && y<0)
					return 270 + tmp;
				else if (x<0 && y>0)
					return 90 + tmp;

				return tmp;
			}

		};

		typedef vector2d<f32> vector2df;
		typedef vector2d<s32> vector2di;

		YON_API extern const vector2df ORIGIN_VECTOR2DF;
		YON_API extern const vector2df IDENTITY_VECTOR2DF;
		YON_API extern const vector2di ORIGIN_VECTOR2DI;
		YON_API extern const vector2di IDENTITY_VECTOR2DI;
	}//core
}//yon
#endif