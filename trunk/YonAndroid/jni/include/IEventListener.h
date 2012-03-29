#ifndef _YON_EVENT_IEVENTLISTENER_H_
#define _YON_EVENT_IEVENTLISTENER_H_

namespace yon{
	namespace event{
		enum ENUM_EVENT_TYPE{
			ENUM_EVENT_TYPE_TOUCH = 0,
			ENUM_EVENT_TYPE_GUI,
			ENUM_EVENT_TYPE_COUNT
		};

		enum ENUM_TOUCH_EVENT_TYPE{
			ENUM_TOUCH_EVENT_TYPE_DOWN = 0,
			ENUM_TOUCH_EVENT_TYPE_UP,
			ENUM_TOUCH_EVENT_TYPE_COUNT
		};
		struct SEvent{
		};

		class IEventListener{
		public:
			virtual ~IEventListener(){}
			virtual bool OnEvent(const SEvent& event) = 0;
		};
	}
}
#endif