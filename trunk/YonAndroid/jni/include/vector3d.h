#ifndef _YON_CORE_VECTOR3D_H_
#define _YON_CORE_VECTOR3D_H_

#include "yonConfig.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
namespace core{

	template<class T>
	class vector3d{
	public:
		T x;
		T y;
		T z;
		vector3d():x(0),y(0),z(0){}
		vector3d(T x,T y,T z):x(x),y(y),z(z){}
		vector3d(const vector3d<T>& other) : x(other.x), y(other.y), z(other.z) {}

		inline void print(){
			if(Logger){
				Logger->debug("%.3f,%.3f,%.3f\n",x,y,z);
			}else{
				printf("%.3f,%.3f,%.3f\n",x,y,z);
			}
		}

		vector3d<T> operator-() const { return vector3d<T>(-x, -y, -z); }

		vector3d<T>& operator=(const vector3d<T>& other) { x = other.x; y = other.y; z = other.z; return *this; }

		vector3d<T> operator+(const vector3d<T>& other) const { return vector3d<T>(x + other.x, y + other.y, z + other.z); }
		vector3d<T>& operator+=(const vector3d<T>& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
		vector3d<T> operator+(const T val) const { return vector3d<T>(x + val, y + val, z + val); }
		vector3d<T>& operator+=(const T val) { x+=val; y+=val; z+=val; return *this; }

		vector3d<T> operator-(const vector3d<T>& other) const { return vector3d<T>(x - other.x, y - other.y, z - other.z); }
		vector3d<T>& operator-=(const vector3d<T>& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
		vector3d<T> operator-(const T val) const { return vector3d<T>(x - val, y - val, z - val); }
		vector3d<T>& operator-=(const T val) { x-=val; y-=val; z-=val; return *this; }

		vector3d<T> operator*(const vector3d<T>& other) const { return vector3d<T>(x * other.x, y * other.y, z * other.z); }
		vector3d<T>& operator*=(const vector3d<T>& other) { x*=other.x; y*=other.y; z*=other.z; return *this; }
		vector3d<T> operator*(const T v) const { return vector3d<T>(x * v, y * v, z * v); }
		vector3d<T>& operator*=(const T v) { x*=v; y*=v; z*=v; return *this; }

		vector3d<T> operator/(const vector3d<T>& other) const { return vector3d<T>(x / other.x, y / other.y, z / other.z); }
		vector3d<T>& operator/=(const vector3d<T>& other) { x/=other.x; y/=other.y; z/=other.z; return *this; }
		vector3d<T> operator/(const T v) const { T i=(T)1.0/v; return vector3d<T>(x * i, y * i, z * i); }
		vector3d<T>& operator/=(const T v) { T i=(T)1.0/v; x*=i; y*=i; z*=i; return *this; }

		bool operator==(const vector3d<T>& other) const
		{
			return this->equals(other);
		}

		bool operator!=(const vector3d<T>& other) const
		{
			return !this->equals(other);
		}

		// functions

		//! returns if this vector equals the other one, taking floating point rounding errors into account
		bool equals(const vector3d<T>& other, const T tolerance = (T)ROUNDING_ERROR_f32 ) const
		{
			return core::equals(x, other.x, tolerance) &&
				core::equals(y, other.y, tolerance) &&
				core::equals(z, other.z, tolerance);
		}


		vector3d<T>& set(const T nx, const T ny, const T nz) {x=nx; y=ny; z=nz; return *this;}
		vector3d<T>& set(const vector3d<T>& p) {x=p.x; y=p.y; z=p.z;return *this;}

		T getLength() const { return core::squareroot( x*x + y*y + z*z ); }

		T getLengthSQ() const { return x*x + y*y + z*z; }

		T getDistanceFrom(const vector3d<T>& other) const
		{
			return vector3d<T>(x - other.x, y - other.y, z - other.z).getLength();
		}

		T getDistanceFromSQ(const vector3d<T>& other) const
		{
			return vector3d<T>(x - other.x, y - other.y, z - other.z).getLengthSQ();
		}

		T dotProduct(const vector3d<T>& other) const
		{
			return x*other.x + y*other.y + z*other.z;
		}

		vector3d<T> crossProduct(const vector3d<T>& p) const
		{
			return vector3d<T>(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
		}

		bool isBetweenPoints(const vector3d<T>& begin, const vector3d<T>& end) const
		{
			const T f = (end - begin).getLengthSQ();
			return getDistanceFromSQ(begin) <= f && getDistanceFromSQ(end) <= f;
		}

		vector3d<T>& normalize(){
			f64 length = x*x + y*y + z*z;
			if (core::equals(length, 0.0)) // this check isn't an optimization but prevents getting NAN in the sqrt.
				return *this;
			//length = core::reciprocal_squareroot(length);
			length = 1.0f/sqrt(length);

			x = (T)(x * length);
			y = (T)(y * length);
			z = (T)(z * length);
			return *this;
		}

		vector3d<T>& setLength(T newlength)
		{
			normalize();
			return (*this *= newlength);
		}
	
		/*return A rotation vector containing the X (pitch) and Y (raw) rotations (in degrees) that when applied to a
		+Z (e.g. 0, 0, 1) direction vector would make it point in the same direction as this vector. The Z (roll) rotation
		is always 0, since two Euler rotations are sufficient to point in any given direction. [0,360)*/
		vector3d<T> getHorizontalAngle() const
		{
			vector3d<T> angle;

			const f64 tmp = (atan2((f64)x, (f64)z) * RADTODEG64);
			angle.y = (T)tmp;

			if (angle.y < 0)
				angle.y += 360;
			if (angle.y >= 360)
				angle.y -= 360;

			const f64 z1 = core::squareroot(x*x + z*z);

			angle.x = (T)(90-atan2((f64)z1, (f64)y) * RADTODEG64);

			/*if (angle.x < 0)
				angle.x += 90;
			if (angle.x >= 90)
				angle.x -= 90;*/

			return angle;
		}
	};

	typedef vector3d<f32> vector3df;

	YON_API extern const vector3df ORIGIN_VECTOR3DF;
	YON_API extern const vector3df UP_VECTOR3DF;
	YON_API extern const vector3df IDENTITY_VECTOR3DF;
}
}
#endif