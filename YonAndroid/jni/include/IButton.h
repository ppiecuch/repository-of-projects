#ifndef _YON_GUI_IBUTTON_
#define _YON_GUI_IBUTTON_

#include "IWidget.h"
#include "IInputable.h"

namespace yon{
namespace gui{
	
	class IButton : public IWidget, public IInputable{
	public:
		IButton(IWidget::ENUM_TYPE type,IGUISystem* system,IWidget* parent,const core::stringc& id,const core::recti& rectangle)
			:IWidget(type,system,parent,id,rectangle){}

		virtual MASK_STATE getState() const = 0;

		virtual IWidget::ENUM_TYPE getType() const{
			return IWidget::BUTTON;
		}

	};
}
}
#endif