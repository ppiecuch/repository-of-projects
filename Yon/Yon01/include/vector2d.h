#ifndef _YON_CORE_VECTOR2D_H_
#define _YON_CORE_VECTOR2D_H_

#include "yonTypes.h"

namespace yon{
	namespace core{

		template<class T>
		class vector2d{
		public:
			T x,y;
			vector2d():x(0),y(0){}
			vector2d(T x,T y):x(x),y(y){}
			vector2d(const vector2d<T>& other) : x(other.x), y(other.y){}
		};

		typedef vector2d<f32> vector2df;
	}//core
}//yon
#endif