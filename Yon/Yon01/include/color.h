 #ifndef _YON_CORE_COLOR_H_
#define _YON_CORE_COLOR_H_

#include "config.h"
#include "yonTypes.h"

namespace yon{
	namespace core{

		//TODO
		/*
		template<class T,size_t R,size_t G,size_t B,size_t A>
		class color{
			color():colour(0){}
			color(u32 r,u32 g,u32 b,u32 a)
			{}
			T colour;
		};
		*/

		class color{
		public:
			color():colour(0){}
			color(u32 r,u32 g,u32 b,u32 a)
				:colour((r & 0xff)<<24 | (g & 0xff)<<16 | (b & 0xff)<<8 | (a & 0xff)) {}
			color(u32 c):colour(c){}

			u32 getRed() const { return colour>>24; }
			u32 getGreen() const { return colour>>16&0xff; }
			u32 getBlue() const { return colour>>8&0xff; }
			u32 getAlpha() const { return colour&0xff; }
			u32 colour;
		};

		YON_API extern const color COLOR_GRAY;
	}
}
#endif