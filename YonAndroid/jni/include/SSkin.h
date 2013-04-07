#ifndef _YON_GUI_SSKIN_H_
#define _YON_GUI_SSKIN_H_

#include "rect.h"

namespace yon{
namespace video{
	class ITexture;
}
namespace gui{

	struct SSkin{
	public:
		video::ITexture* Texture;
		core::rectf Rectangle;
		//bool Repeat;
		//bool Transparent;

		SSkin& build(video::ITexture* texture,s32 x,s32 y,s32 width,s32 height){
			YON_DEBUG_BREAK_IF(texture==NULL);
			Texture=texture;
			f32 x0=x/(f32)texture->getSize().w;
			f32 y0=y/(f32)texture->getSize().h;
			f32 x1=(x+width)/(f32)texture->getSize().w;
			f32 y1=(y+height)/(f32)texture->getSize().h;
			Rectangle.set(x0,y0,x1,y1);
			return *this;
		}
	};
}
}
#endif