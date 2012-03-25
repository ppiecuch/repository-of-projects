#ifndef _YON_CORE_VECTOR3D_H_
#define _YON_CORE_VECTOR3D_H_

#include "config.h"
#include "yonMath.h"

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

			vector3d<T>& set(const T nx, const T ny, const T nz) {x=nx; y=ny; z=nz; return *this;}
			vector3d<T>& set(const vector3d<T>& p) {x=p.x; y=p.y; z=p.z;return *this;}


			T dotProduct(const vector3d<T>& other) const
			{
				return x*other.x + y*other.y + z*other.z;
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

		};

		typedef vector3d<f32> vector3df;

		YON_API extern const vector3df UP_VECTOR3DF;
	}//core
}//yon
#endif