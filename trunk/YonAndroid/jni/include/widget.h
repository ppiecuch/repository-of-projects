#ifndef _YON_GUI_WIDGET_H_
#define _YON_GUI_WIDGET_H_

#include "yonTypes.h"

namespace yon{
namespace gui{

	namespace widget{

		enum ENUM_TYPE{
			NONE = -1,
			BUTTON = 0,
			ENUM_TYPE_COUNT
		};

		//TODO 水平垂直分开
		enum ENUM_ALIGN{
			//! Aligned to parent's top or left side (default)
			UPPERLEFT=0,
			//! Aligned to parent's bottom or right side
			LOWERRIGHT,
			//! Aligned to the center of parent
			CENTER,
			ENUM_ALIGN_COUNT
		};

		enum ENUM_STATE{
			UNKNOWN	= 0,
			DOWN	= 0x00000001,
			UP		= 0x00000002,
			HOVER	= 0x00000004,
			LEAVE	= 0x00000008,
			FOCUS	= 0x00000010,
			BLUR	= 0x00000020,
			//TODO 长按
			//HOLD
			ENUM_STATE_COUNT = 7
		};

		enum ENUM_SLICE{
			ONE_REGION = 0,
			NINE_GRID,
			HORIZONTAL,
			VERTICAL,
			ENUM_SLICE_COUNT
		};

		enum ENUM_AZIM{
			CNT = 0,
			NTH,
			NE,
			EST,
			SE,
			STH,
			SW,
			WST,
			NW,
			ENUM_AZIM_COUNT
		};

		const static c8* TYPE_NAMES[ENUM_TYPE_COUNT] =
		{
			"button"
		};
		const static c8* ALIGN_NAMES[ENUM_ALIGN_COUNT] =
		{
			"upperleft",
			"lowerright",
			"center"
		};
		const static c8* AZIM_NAMES[ENUM_AZIM_COUNT] =
		{
			"center",
			"north",
			"northeast",
			"east",
			"southeast",
			"south",
			"southwest",
			"west",
			"northwest"
		};

		//TODO MASK_STATE是非递增枚举
		/*const static c8* STATE_NAMES[ENUM_STATE_COUNT]=
		{
			"down",
			"up",
			"over",
			"out",
			"focus"
		};
		*/
	}
}
}
#endif