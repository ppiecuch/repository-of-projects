#ifndef _YON_VIDEO_IVIDEORENDERER_H_
#define _YON_VIDEO_IVIDEORENDERER_H_

#include "IReferencable.h"
#include "color.h"
#include "rect.h"

namespace yon{
	namespace video{

		//ÊÓÆµäÖÈ¾Æ÷½Ó¿Ú
		class IVideoRenderer:public virtual IReferencable{
		public:
			virtual ~IVideoRenderer(){};
			virtual void begin(bool zBuffer=true,core::color c=core::color(0x000000FF)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const core::recti& rec) = 0;
			virtual u32 getFPS() const = 0;
		};
	}
}
#endif