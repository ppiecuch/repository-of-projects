#ifndef _YON_GUI_ITHEME_H_
#define _YON_GUI_ITHEME_H_

#include "IReferencable.h"
#include "IWidget.h"

namespace yon{
namespace gui{

	struct SSkin;
	
	/**
	* Stores collections of Skin objects.
	*/
	class ITheme : public virtual core::IReferencable{
	public:
		
		virtual bool addSkin(IWidget::ENUM_TYPE type,IWidget::ENUM_STATE state,s32 index, const SSkin& skin) = 0;
		virtual const SSkin& getSkin(IWidget::ENUM_TYPE type,IWidget::ENUM_STATE state,s32 index=0) const = 0;

		virtual const core::stringc& getName() const = 0;

	};
}
}
#endif