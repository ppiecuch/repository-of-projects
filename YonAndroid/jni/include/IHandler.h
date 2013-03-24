#ifndef _YON_GUI_IHANDLER_H_
#define _YON_GUI_IHANDLER_H_

#include "widget.h"

namespace yon{
namespace gui{

	class IWidget;

	struct SMessage{
		//! who called the message
		IWidget* Caller;

		//! If the message has something to do with another element, it will be held here.
		IWidget* Widget;

		//! Type of Message
		widget::ENUM_STATE Type;
	};

	class IHandler{
	public:
		virtual bool handleMessage(const SMessage& msg) = 0;
		virtual bool isMessageReceivable() const{return true;}
	};
}
}
#endif