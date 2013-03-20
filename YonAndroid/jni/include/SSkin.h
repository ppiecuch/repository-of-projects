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
	};
}
}
#endif