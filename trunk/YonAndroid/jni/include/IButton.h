#ifndef _YON_GUI_IBUTTON_
#define _YON_GUI_IBUTTON_

#include "IWidget.h"
#include "IInputable.h"

namespace yon{
namespace gui{
	
	class IButton : public virtual IWidget, public IInputable{
	public:

		virtual ENUM_STATE getState() const = 0;

		virtual IWidget::ENUM_TYPE getType() const{
			return BUTTON;
		}

	};
}
}
#endif