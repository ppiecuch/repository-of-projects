#ifndef _YON_CORE_RECT_H_
#define _YON_CORE_RECT_H_

#include "yonTypes.h"
#include "dimension2d.h"
#include "position2d.h"

#include "ILogger.h"

namespace yon{
	namespace core{
		
		template<class T>
		class rect{
		public:
			/*rect():x(0),y(0),w(0),h(0){}
			rect(T x,T y,T w,T h):x(x),y(y),w(w),h(h){}
			T x,y,w,h;*/


			//0--------->x
			//|
			//|
			//|
			//\/y

			rect() : topLeft(0,0), bottomRight(0,0) {}
			rect(T x, T y, T x2, T y2)
				: topLeft(x,y), bottomRight(x2,y2) {}
			rect(const position2d<T>& topLeft, const position2d<T>& bottomRight)
				: topLeft(topLeft), bottomRight(bottomRight) {}
			template <class U>
			rect(const position2d<T>& pos, const dimension2d<U>& size)
				: topLeft(pos), bottomRight(pos.x + size.w, pos.y + size.h) {}

			void print() const {
				Logger->debug("{%.3f,%.3f,%.3f,%.3f}\n",(f32)topLeft.x,(f32)topLeft.y,(f32)bottomRight.x,(f32)bottomRight.y);
			}

			rect<T> operator+(const position2d<T>& pos) const
			{
				rect<T> ret(*this);
				return ret+=pos;
			}
			rect<T>& operator+=(const position2d<T>& pos)
			{
				topLeft += pos;
				bottomRight += pos;
				return *this;
			}
			rect<T> operator-(const position2d<T>& pos) const
			{
				rect<T> ret(*this);
				return ret-=pos;
			}
			rect<T>& operator-=(const position2d<T>& pos)
			{
				topLeft -= pos;
				bottomRight -= pos;
				return *this;
			}
			bool operator==(const rect<T>& other) const
			{
				return (topLeft == other.topLeft &&
					bottomRight == other.bottomRight);
			}
			bool operator!=(const rect<T>& other) const
			{
				return (topLeft != other.topLeft ||
					bottomRight != other.bottomRight);
			}
			bool operator<(const rect<T>& other) const
			{
				return getArea() < other.getArea();
			}

			//ÑØ×ÅotherÇøÓò²Ã¼ô±¾rect
			void clipAgainst(const rect<T>& other)
			{
				if (other.bottomRight.x < bottomRight.x)
					bottomRight.x = other.bottomRight.x;
				if (other.bottomRight.y < bottomRight.y)
					bottomRight.y = other.bottomRight.y;

				if (other.topLeft.x > topLeft.x)
					topLeft.x = other.topLeft.x;
				if (other.topLeft.y > topLeft.y)
					topLeft.y = other.topLeft.y;

				if (topLeft.y > bottomRight.y)
					topLeft.y = bottomRight.y;
				if (topLeft.x > bottomRight.x)
					topLeft.x = bottomRight.x;
			}

			bool isValid() const
			{
				return ((bottomRight.x >= topLeft.x) &&
					(bottomRight.y >= topLeft.y));
			}

			T getWidth() const
			{
				return bottomRight.x - topLeft.x;
			}
			T getHeight() const
			{
				return bottomRight.y - topLeft.y;
			}
			T getArea() const
			{
				return getWidth() * getHeight();
			}
			dimension2d<T> getSize() const
			{
				return dimension2d<T>(getWidth(), getHeight());
			}

			position2d<T> topLeft;
			position2d<T> bottomRight;

			//position2d<T> bottomLeft;
			//position2d<T> topRight;
		};



		typedef rect<f32> rectf;
		typedef rect<s32> recti;
	}
}
#endif