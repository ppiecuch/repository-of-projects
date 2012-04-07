#ifndef _YON_EVENT_IEVENTRECEIVER_H_
#define _YON_EVENT_IEVENTRECEIVER_H_

namespace yon{
	namespace event{
		enum ENUM_EVENT_TYPE{
			ENUM_EVENT_TYPE_TOUCH = 0,
			ENUM_EVENT_TYPE_MOUSE,
			ENUM_EVENT_TYPE_GUI,
			ENUM_EVENT_TYPE_COUNT
		};

		enum ENUM_TOUCH_INPUT_TYPE{
			ENUM_TOUCH_INPUT_TYPE_DOWN = 0,
			ENUM_TOUCH_INPUT_TYPE_UP,
			ENUM_TOUCH_INPUT_TYPE_MOVE,
			ENUM_TOUCH_INPUT_TYPE_COUNT
		};

		enum ENUM_MOUSE_INPUT_TYPE
		{
			ENUM_MOUSE_INPUT_TYPE_LDOWN = 0,
			ENUM_MOUSE_INPUT_TYPE_RDOWN,
			ENUM_MOUSE_INPUT_TYPE_MDOWN,

			ENUM_MOUSE_INPUT_TYPE_LUP,
			ENUM_MOUSE_INPUT_TYPE_RUP,
			ENUM_MOUSE_INPUT_TYPE_MUP,

			ENUM_MOUSE_INPUT_TYPE_MOVED,
			ENUM_MOUSE_INPUT_TYPE_WHEEL,

			//TODO
			//ENUM_MOUSE_INPUT_LDBCLICK,
			//ENUM_MOUSE_INPUT_RDBCLICK,
			//ENUM_MOUSE_INPUT_MDBCLICK,

			//ENUM_MOUSE_INPUT_LTBCLICK,
			//ENUM_MOUSE_INPUT_RTBCLICK,
			//ENUM_MOUSE_INPUT_MTBCLICK,

			ENUM_MOUSE_INPUT_TYPE_COUNT
		};

		enum MASK_MOUSE_BUTTON{
			MASK_MOUSE_BUTTON_LEFT = 0x01,
			MASK_MOUSE_BUTTON_MIDDLE = 0x02,
			MASK_MOUSE_BUTTON_RIGHT = 0x04,
			MASK_MOUSE_BUTTON_COUNT
		};

		struct STouchInput{
			ENUM_TOUCH_INPUT_TYPE type;
			f32 x,y;
		};
		struct SMouseInput{
			ENUM_MOUSE_INPUT_TYPE type;
			s32 x,y;
			f32 wheel;
			u32 buttonMasks;

			bool isLeftPressed() const { return 0 != ( buttonMasks & MASK_MOUSE_BUTTON_LEFT ); }
			bool isMiddlePressed() const { return 0 != ( buttonMasks & MASK_MOUSE_BUTTON_MIDDLE ); }
			bool isRightPressed() const { return 0 != ( buttonMasks & MASK_MOUSE_BUTTON_RIGHT ); }
		};

		struct SEvent{
			ENUM_EVENT_TYPE type;
			union{
				STouchInput touchInput;
				SMouseInput mouseInput;
			};
		};

		class IEventReceiver{
		public:
			virtual ~IEventReceiver(){}
			virtual bool onEvent(const SEvent& event) = 0;
		};
	}
}
#endif