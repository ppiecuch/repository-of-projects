#ifndef _YON_CORE_RECT_H_
#define _YON_CORE_RECT_H_

#include "yonTypes.h"

namespace yon{
	namespace core{
		
		template<class T>
		class rect{
		public:
			rect():x(0),y(0),w(0),h(0){}
			rect(T x,T y,T w,T h):x(x),y(y),w(w),h(h){}

			T x,y,w,h;
		};
		typedef rect<f32> rectf;
		typedef rect<s32> recti;
	}
}
#endif