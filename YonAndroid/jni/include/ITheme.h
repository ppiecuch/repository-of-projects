#ifndef _YON_GUI_ITHEME_H_
#define _YON_GUI_ITHEME_H_

#include "IReferencable.h"
#include "IWidget.h"

namespace yon{
namespace gui{

	class ISkin;
	
	/**
	* Stores collections of Skin objects.
	*/
	class ITheme : public virtual core::IReferencable{
	public:
		
		virtual ISkin* getSkin(IWidget::ENUM_TYPE type,IWidget::ENUM_STATE state) = 0;

		virtual const core::stringc& getName() const = 0;

	};
}
}
#endif