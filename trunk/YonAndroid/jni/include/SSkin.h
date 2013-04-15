#ifndef _YON_GUI_SSKIN_H_
#define _YON_GUI_SSKIN_H_

#include "rect.h"
#include "SSlice.h"

namespace yon{
namespace gui{

	struct SSkin{
	public:
		core::stringc Name;
		widget::ENUM_TYPE Type;
		SSlice* Slices[widget::ENUM_AZIM_COUNT];

		video::ITexture* Texture;
		core::rectf Rectangle;
		//bool Repeat;
		//bool Transparent;
	};
}
}
#endif