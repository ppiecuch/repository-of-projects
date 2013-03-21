#ifndef _YON_GUI_ITHEME_H_
#define _YON_GUI_ITHEME_H_

#include "IReferencable.h"
#include "IWidget.h"
#include "yonMath.h"

namespace yon{
namespace gui{

	struct SSkin;

	struct SWidgetState
	{
		IWidget::ENUM_TYPE Type;
		s32 StateMask;
	};

	const static SWidgetState WIDGET_STATES[IWidget::ENUM_TYPE_COUNT]={
		{IWidget::BUTTON,	IWidget::DOWN|IWidget::UP|IWidget::OVER|IWidget::OUT}
	};

	/**
	* Stores collections of Skin objects.
	*/
	class ITheme : public virtual core::IReferencable{
	public:
		
		virtual bool addSkin(IWidget::ENUM_TYPE type,IWidget::MASK_STATE state,s32 index, const SSkin& skin) = 0;
		virtual const SSkin& getSkin(IWidget::ENUM_TYPE type,IWidget::MASK_STATE state,s32 index=0) const = 0;

		virtual const core::stringc& getName() const = 0;

		static u32 getStateCount(IWidget::ENUM_TYPE type);

	};

	u32 ITheme::getStateCount(IWidget::ENUM_TYPE type)
	{
		return core::countBits(WIDGET_STATES[type].StateMask);
	}
}
}
#endif