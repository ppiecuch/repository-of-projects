#ifndef _YON_EVENT_SKEYMAP_H_
#define _YON_EVENT_SKEYMAP_H_

#include "keycodes.h"

namespace yon{
namespace event{

	enum ENUM_ACTION{
		ACTION_MOVE_FORWARD=0,
		ACTION_MOVE_BACKWARD,
		ACTION_MOVE_LEFTWARD,
		ACTION_MOVE_RIGHTWARD,
		ACTION_COUNT
	};

	struct SKeyMap{
		SKeyMap(ENUM_ACTION action,ENUM_KEY key):action(action),keycode(key){}

		ENUM_ACTION action;
		ENUM_KEY keycode;
	};
}
}
#endif