#ifndef _YON_GUI_IWIDGET_H_
#define _YON_GUI_IWIDGET_H_

#include "IReferencable.h"
#include "yonString.h"
#include "IGUISystem.h"
#include "position2d.h"

//refer to:http://www.php100.com/manual/css3_0

namespace yon{
namespace gui{

	class IWidget : public virtual core::IReferencable,public virtual event::IEventReceiver{
	public:

		enum ENUM_TYPE{
			BUTTON = 0,
			ENUM_TYPE_COUNT
		};

		enum ENUM_STATE{
			DOWN = 0,
			UP,
			OVER,
			OUT,
			//TODO 长按
			//HOLD
			ENUM_STATE_COUNT
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

		const static c8* TypeNames[ENUM_TYPE_COUNT] =
		{
			"button"
		};

		const static c8* AlignNames[ENUM_TYPE_COUNT] =
		{
			"upperleft",
			"lowerright",
			"center"
		};

		const static c8* StateNames[ENUM_STATE_COUNT]={
			"down",
			"up",
			"over",
			"out"
		};
		//const core::stringc& getTag() const{
			//TODO
		//}

		virtual bool isVisible() const= 0;

		virtual IWidget::ENUM_TYPE getType() const = 0;

		virtual const core::stringc& getId() const = 0;

		virtual IWidget* getParent() const = 0;

		virtual IGUISystem* getGUISystem() const = 0;

		virtual const core::recti& getAbsoluteRectangle() const = 0;

		virtual bool isPointInside(const core::position2di& point) const = 0;
	};
}
}
#endif