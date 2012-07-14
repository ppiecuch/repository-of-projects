#ifndef _YON_CORE_VECTOR2D_H_
#define _YON_CORE_VECTOR2D_H_

#include "yonConfig.h"
#include "yonTypes.h"
#include "yonMath.h"
#include "dimension2d.h"

namespace yon{
	namespace core{

		template<class T>
		class vector2d{
		public:
			T x,y;
			vector2d():x(0),y(0){}
			vector2d(T x,T y):x(x),y(y){}
			explicit vector2d(T n) : x(n), y(n) {}
			vector2d(const vector2d<T>& other) : x(other.x), y(other.y){}
			vector2d(const dimension2d<T>& other) : x(other.w), y(other.h) {}

			vector2d<T> operator-() const { return vector2d<T>(-x, -y); }

			vector2d<T>& operator=(const vector2d<T>& other) { x = other.x; y = other.y; return *this; }

			vector2d<T>& operator=(const dimension2d<T>& other) { x = other.w; y = other.h; return *this; }

			vector2d<T> operator+(const vector2d<T>& other) const { return vector2d<T>(x + other.x, y + other.y); }
			vector2d<T> operator+(const dimension2d<T>& other) const { return vector2d<T>(x + other.w, y + other.h); }
			vector2d<T>& operator+=(const vector2d<T>& other) { x+=other.x; y+=other.y; return *this; }
			vector2d<T> operator+(const T v) const { return vector2d<T>(x + v, y + v); }
			vector2d<T>& operator+=(const T v) { x+=v; y+=v; return *this; }
			vector2d<T>& operator+=(const dimension2d<T>& other) { x += other.w; y += other.h; return *this;  }

			vector2d<T> operator-(const vector2d<T>& other) const { return vector2d<T>(x - other.x, y - other.y); }
			vector2d<T> operator-(const dimension2d<T>& other) const { return vector2d<T>(y - other.w, y - other.h); }
			vector2d<T>& operator-=(const vector2d<T>& other) { x-=other.x; y-=other.y; return *this; }
			vector2d<T> operator-(const T v) const { return vector2d<T>(x - v, y - v); }
			vector2d<T>& operator-=(const T v) { x-=v; y-=v; return *this; }
			vector2d<T>& operator-=(const dimension2d<T>& other) { x -= other.w; y -= other.h; return *this;  }

			vector2d<T> operator*(const vector2d<T>& other) const { return vector2d<T>(x * other.x, y * other.y); }
			vector2d<T>& operator*=(const vector2d<T>& other) { x*=other.x; y*=other.y; return *this; }
			vector2d<T> operator*(const T v) const { return vector2d<T>(x * v, y * v); }
			vector2d<T>& operator*=(const T v) { x*=v; y*=v; return *this; }

			vector2d<T> operator/(const vector2d<T>& other) const { return vector2d<T>(x / other.x, y / other.y); }
			vector2d<T>& operator/=(const vector2d<T>& other) { x/=other.x; y/=other.y; return *this; }
			vector2d<T> operator/(const T v) const { return vector2d<T>(x / v, y / v); }
			vector2d<T>& operator/=(const T v) { x/=v; y/=v; return *this; }

			//! sort in order X, Y. Equality with rounding tolerance.
			bool operator<=(const vector2d<T>&other) const
			{
				return 	(x<other.x || core::equals(x, other.x)) ||
					(core::equals(x, other.x) && (y<other.y || core::equals(y, other.y)));
			}

			//! sort in order X, Y. Equality with rounding tolerance.
			bool operator>=(const vector2d<T>&other) const
			{
				return 	(x>other.x || core::equals(x, other.x)) ||
					(core::equals(x, other.x) && (y>other.y || core::equals(y, other.y)));
			}

			//! sort in order X, Y. Difference must be above rounding tolerance.
			bool operator<(const vector2d<T>&other) const
			{
				return 	(x<other.x && !core::equals(x, other.x)) ||
					(core::equals(x, other.x) && y<other.y && !core::equals(y, other.y));
			}

			//! sort in order X, Y. Difference must be above rounding tolerance.
			bool operator>(const vector2d<T>&other) const
			{
				return 	(x>other.x && !core::equals(x, other.x)) ||
					(core::equals(x, other.x) && y>other.y && !core::equals(y, other.y));
			}

			bool operator==(const vector2d<T>& other) const { return equals(other); }
			bool operator!=(const vector2d<T>& other) const { return !equals(other); }

			bool equals(const vector2d<T>& other) const
			{
				return core::equals(x, other.x) && core::equals(y, other.y);
			}

			inline void print(){
				printf("%.3f,%.3f\n",x,y);
			}

			vector2d<T>& set(const T nx, const T ny) {x=nx; y=ny; return *this;}
			vector2d<T>& set(const vector2d<T>& p) {x=p.x; y=p.y; return *this;}

			//! Gets the length of the vector.
			/** \return The length of the vector. */
			T getLength() const { return core::squareroot( x*x + y*y ); }

			//! Get the squared length of this vector
			/** This is useful because it is much faster than getLength().
			\return The squared length of the vector. */
			T getLengthSQ() const { return x*x + y*y; }

			//! Get the dot product of this vector with another.
			/** \param other Other vector to take dot product with.
			\return The dot product of the two vectors. */
			T dotProduct(const vector2d<T>& other) const
			{
				return x*other.x + y*other.y;
			}

			//! Gets distance from another point.
			/** Here, the vector is interpreted as a point in 2-dimensional space.
			\param other Other vector to measure from.
			\return Distance from other point. */
			T getDistanceFrom(const vector2d<T>& other) const
			{
				return vector2d<T>(x - other.x, y - other.y).getLength();
			}

			//! Returns squared distance from another point.
			/** Here, the vector is interpreted as a point in 2-dimensional space.
			\param other Other vector to measure from.
			\return Squared distance from other point. */
			T getDistanceFromSQ(const vector2d<T>& other) const
			{
				return vector2d<T>(x - other.x, y - other.y).getLengthSQ();
			}

			vector2d<T>& rotateBy(f32 radian, const vector2d<T>& center=vector2d<T>())
			{
				const f32 cs = cos(radian);
				const f32 sn = sin(radian);

				x -= center.x;
				y -= center.y;

				set((T)(x*cs - y*sn), (T)(x*sn + y*cs));

				x += center.x;
				y += center.y;
				return *this;
			}

			vector2d<T>& rotateByDegree(f32 degrees, const vector2d<T>& center=vector2d<T>())
			{
				degrees *= DEGTORAD;
				return rotateBy(degrees);
			}

			vector2d<T>& normalize()
			{
				f32 length = (f32)(x*x + y*y);
				if (core::equals(length, 0.f))
					return *this;
				length = core::reciprocal_squareroot ( length );
				x = (T)(x * length);
				y = (T)(y * length);
				return *this;
			}

			//计算向量在屏幕坐标系中的角度
			//! Calculates the angle of this vector in degrees in the trigonometric sense.
			/** 0 is to the right (3 o'clock), values increase counter-clockwise.
			This method has been suggested by Pr3t3nd3r.
			\return Returns a value between 0 and 360. */
			f64 getAngleTrig() const
			{
				if (y == 0)
					return x < 0 ? 180 : 0;
				else
					if (x == 0)
						return y < 0 ? 270 : 90;

				if ( y > 0)
					if (x > 0)
						return atan((f64)y/(f64)x) * RADTODEG64;
					else
						return 180.0-atan((f64)y/-(f64)x) * RADTODEG64;
				else
					if (x > 0)
						return 360.0-atan(-(f64)y/(f64)x) * RADTODEG64;
					else
						return 180.0+atan(-(f64)y/-(f64)x) * RADTODEG64;
			}

			//! Returns if this vector interpreted as a point is on a line between two other points.
			/** It is assumed that the point is on the line.
			\param begin Beginning vector to compare between.
			\param end Ending vector to compare between.
			\return True if this vector is between begin and end, false if not. */
			bool isBetweenPoints(const vector2d<T>& begin, const vector2d<T>& end) const
			{
				if (begin.x != end.x)
				{
					return ((begin.x <= x && x <= end.x) ||
						(begin.x >= x && x >= end.x));
				}
				else
				{
					return ((begin.y <= y && y <= end.y) ||
						(begin.y >= y && y >= end.y));
				}
			}


			vector2d<T>& interpolate(const vector2d<T>& a, const vector2d<T>& b, f64 d)
			{
				x = (T)((f64)b.x + ( ( a.x - b.x ) * d ));
				y = (T)((f64)b.y + ( ( a.y - b.y ) * d ));
				return *this;
			}


			//----->x
			//|
			//|
			//y
			//范围：[0,2PI)
			inline f32 getAngle() const
			{
				if (y == 0) // corrected thanks to a suggestion by Jox
					return x < 0 ? xc::core::PI : 0;
				else if (x == 0)
					return y < 0 ? (xc::core::PI + xc::core::PI / 2) : (xc::core::PI / 2);

				// don't use getLength here to avoid precision loss with s32 vectors
				f32 tmp = y / sqrt((f32)(x*x + y*y));
				tmp = atan( core::squareroot(1 - tmp*tmp) / tmp);

				if (x>0 && y>0)
					return xc::core::PI / 2 - tmp;
				else if (x>0 && y<0)
					return (xc::core::PI + xc::core::PI / 2) - tmp;
				else if (x<0 && y<0)
					return (xc::core::PI + xc::core::PI / 2) + tmp;
				else if (x<0 && y>0)
					return xc::core::PI / 2 + tmp;

				return tmp;
			}

			//----->x
			//|
			//|
			//y
			//范围：[0,360)
			inline f32 getAngleDegree() const
			{
				return getAngle() * RADTODEG;
			}

		};

		typedef vector2d<f32> vector2df;
		typedef vector2d<s32> vector2di;

		template<class S, class T>
		vector2d<T> operator*(const S scalar, const vector2d<T>& vector) { return vector*scalar; }

		// These methods are declared in dimension2d, but need definitions of vector2d
		template<class T>
		dimension2d<T>::dimension2d(const vector2d<T>& other) : w(other.x), h(other.y) {}

		template<class T>
		bool dimension2d<T>::operator==(const vector2d<T>& other) const { return w == other.x && h == other.y; }

		YON_API extern const vector2df ORIGIN_VECTOR2DF;
		YON_API extern const vector2df IDENTITY_VECTOR2DF;
		YON_API extern const vector2di ORIGIN_VECTOR2DI;
		YON_API extern const vector2di IDENTITY_VECTOR2DI;
	}//core
}//yon
#endif