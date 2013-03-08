#ifndef _YON_GUI_IMESSAGE_H_
#define _YON_GUI_IMESSAGE_H_

#include "IReferencable.h"

namespace yon{
namespace gui{

	enum ENUM_MESSAGE_TYPE{
		ENUM_MESSAGE_TYPE_NONE = 0,
		ENUM_MESSAGE_TYPE_WIDGET,
		ENUM_MESSAGE_TYPE_MOUSE,
		ENUM_MESSAGE_TYPE_KEY,
		ENUM_MESSAGE_TYPE_SCROLL,
		ENUM_MESSAGE_TYPE_TEXT,
		ENUM_MESSAGE_TYPE_TIME,
		ENUM_MESSAGE_TYPE_COUNT
	};

	class IMessage : public virtual core::IReferencable{

	};
}
}
#endif