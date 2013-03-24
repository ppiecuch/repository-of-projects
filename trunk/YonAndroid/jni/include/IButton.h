#ifndef _YON_GUI_IBUTTON_
#define _YON_GUI_IBUTTON_

#include "IWidget.h"
#include "IInputable.h"

namespace yon{
namespace gui{
	
	class IButton : public IWidget, public IInputable{
	public:
		IButton(widget::ENUM_TYPE type,IGUISystem* system,IWidget* parent,const core::stringc& id,const core::recti& rectangle)
			:IWidget(type,system,parent,id,rectangle){}

		virtual widget::ENUM_STATE getState() const = 0;

		virtual widget::ENUM_TYPE getType() const{
			return widget::BUTTON;
		}

	};
}
}
#endif