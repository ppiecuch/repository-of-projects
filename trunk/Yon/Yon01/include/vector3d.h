#ifndef _YON_CORE_VECTOR3D_H_
#define _YON_CORE_VECTOR3D_H_

#include "vector3d.h"

namespace yon{
	namespace core{

		template<class T>
		class vector3d{
		public:
			T x,y,z;
			vector3d():x(0),y(0),z(0){}
			vector3d(T x,T y,T z):x(x),y(y),z(z){}
			vector3d(const vector3d<T>& other) : x(other.x), y(other.y), z(other.z) {}
		};

		typedef vector3d<f32> vector3df;
	}
}
#endif