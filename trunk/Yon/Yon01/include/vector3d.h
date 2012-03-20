#ifndef _YON_CORE_VECTOR3D_H_
#define _YON_CORE_VECTOR3D_H_

#include "config.h"
#include "vector3d.h"

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

			T dotProduct(const vector3d<T>& other) const
			{
				return x*other.x + y*other.y + z*other.z;
			}
		};

		typedef vector3d<f32> vector3df;

		YON_API extern const vector3df UP_VECTOR3DF;
	}//core
}//yon
#endif