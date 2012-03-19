#ifndef _YON_EVENT_IEVENTLISTENER_H_
#define _YON_EVENT_IEVENTLISTENER_H_

namespace yon{
	namespace event{
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