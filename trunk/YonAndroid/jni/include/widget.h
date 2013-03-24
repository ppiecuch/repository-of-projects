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

		enum MASK_STATE{
			UNKNOWN	= 0,
			DOWN	= 0x00000001,
			UP		= 0x00000002,
			OVER	= 0x00000004,
			OUT		= 0x00000008,
			FOCUS	= 0x00000010,
			//TODO 长按
			//HOLD
			ENUM_STATE_COUNT
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