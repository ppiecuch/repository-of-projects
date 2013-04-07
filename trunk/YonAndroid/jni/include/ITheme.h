#ifndef _YON_GUI_ITHEME_H_
#define _YON_GUI_ITHEME_H_

#include "IReferencable.h"
#include "IWidget.h"
#include "yonMath.h"
#include "widget.h"

namespace yon{
namespace gui{

	struct SSkin;

	struct SWidgetState
	{
		widget::ENUM_TYPE Type;
		s32 StateMask;
	};

	const static SWidgetState WIDGET_STATES[widget::ENUM_TYPE_COUNT]={
		{widget::BUTTON,	widget::DOWN|widget::UP|widget::HOVER|widget::LEAVE}
	};

	/**
	 * Stores collections of Skin objects.
	 */
	class ITheme : public virtual core::IReferencable{
	public:
		
		virtual bool addSkin(widget::ENUM_TYPE type,widget::ENUM_STATE state,widget::ENUM_AZIM azim, const SSkin& skin) = 0;

		virtual SSkin getSkin(widget::ENUM_TYPE type,widget::ENUM_STATE state,widget::ENUM_AZIM azim=widget::CNT) const = 0;

		virtual const core::stringc& getName() const = 0;

		static u32 getStateCount(widget::ENUM_TYPE type){return core::countBits(WIDGET_STATES[type].StateMask);}

	};
}
}
#endif