#ifndef _YON_PLATFORM_ICALLBACK_H_
#define _YON_PLATFORM_ICALLBACK_H_

#include "yonTypes.h"

namespace yon{
namespace platform{

	//平台回调接口，通过SEvent传递参数

	enum ENUM_CALLBACK_TYPE{
		ENUM_CALLBACK_TYPE_UI = 0,
		ENUM_CALLBACK_TYPE_COUNT
	};

	enum ENUM_CALLBACK_UI_TYPE{
		ENUM_CALLBACK_UI_TYPE_CONFIRM = 0,
		ENUM_CALLBACK_UI_TYPE_COUNT
	};

	struct SCallbackUI{
		ENUM_CALLBACK_UI_TYPE type;
		c8* title;
		c8* content;
		c8* positiveButton;
		c8* negativeButton;
	};

	struct SCallback{
		ENUM_CALLBACK_TYPE type;
		union{
			SCallbackUI ui;
		};
	};

	class ICallback{
	public:
		virtual bool callback(const platform::SCallback& cb) = 0;
	};
}
}
#endif