#ifndef _YON_EVENT_KEYCODES_H_
#define _YON_EVENT_KEYCODES_H_

namespace yon{
namespace event{

	enum ENUM_KEY{
		ENUM_KEY_TAB			= 0x09,  // TAB key
		ENUM_KEY_RETURN         = 0x0D,  // ENTER key
		ENUM_KEY_PAUSE          = 0x13,  // PAUSE key
		ENUM_KEY_CAPITAL        = 0x14,  // CAPS LOCK key
		ENUM_KEY_ESCAPE         = 0x1B,  // ESC key
		ENUM_KEY_SPACE          = 0x20,  // SPACEBAR
		ENUM_KEY_PRIOR          = 0x21,  // PAGE UP key
		ENUM_KEY_NEXT           = 0x22,  // PAGE DOWN key
		ENUM_KEY_END            = 0x23,  // END key
		ENUM_KEY_HOME           = 0x24,  // HOME key
		ENUM_KEY_PRINT          = 0x2A,  // PRINT key
		ENUM_KEY_SNAPSHOT       = 0x2C,  // PRINT SCREEN key
		ENUM_KEY_INSERT         = 0x2D,  // INS key
		ENUM_KEY_DELETE         = 0x2E,  // DEL key
		ENUM_KEY_LWIN           = 0x5B,  // Left Windows key (Microsoft?Natural?keyboard)
		ENUM_KEY_NUMLOCK        = 0x90,  // NUM LOCK key
		ENUM_KEY_SCROLL         = 0x91,  // SCROLL LOCK key

		ENUM_KEY_LEFT           = 0x25,  // LEFT ARROW key
		ENUM_KEY_UP             = 0x26,  // UP ARROW key
		ENUM_KEY_RIGHT          = 0x27,  // RIGHT ARROW key
		ENUM_KEY_DOWN           = 0x28,  // DOWN ARROW key

		ENUM_KEY_SHIFT          = 0x10,  // SHIFT key
		ENUM_KEY_CONTROL        = 0x11,  // CTRL key
		ENUM_KEY_MENU           = 0x12,  // ALT key
		ENUM_KEY_LSHIFT         = 0xA0,  // Left SHIFT key
		ENUM_KEY_RSHIFT         = 0xA1,  // Right SHIFT key
		ENUM_KEY_LCONTROL       = 0xA2,  // Left CONTROL key
		ENUM_KEY_RCONTROL       = 0xA3,  // Right CONTROL key
		ENUM_KEY_LMENU          = 0xA4,  // Left MENU key
		ENUM_KEY_RMENU          = 0xA5,  // Right MENU key

		ENUM_KEY_KEY_0          = 0x30,  // 0 key
		ENUM_KEY_KEY_1          = 0x31,  // 1 key
		ENUM_KEY_KEY_2          = 0x32,  // 2 key
		ENUM_KEY_KEY_3          = 0x33,  // 3 key
		ENUM_KEY_KEY_4          = 0x34,  // 4 key
		ENUM_KEY_KEY_5          = 0x35,  // 5 key
		ENUM_KEY_KEY_6          = 0x36,  // 6 key
		ENUM_KEY_KEY_7          = 0x37,  // 7 key
		ENUM_KEY_KEY_8          = 0x38,  // 8 key
		ENUM_KEY_KEY_9          = 0x39,  // 9 key

		ENUM_KEY_A				= 0x41,  // A key
		ENUM_KEY_B				= 0x42,  // B key
		ENUM_KEY_C				= 0x43,  // C key
		ENUM_KEY_D				= 0x44,  // D key
		ENUM_KEY_E				= 0x45,  // E key
		ENUM_KEY_F				= 0x46,  // F key
		ENUM_KEY_G				= 0x47,  // G key
		ENUM_KEY_H				= 0x48,  // H key
		ENUM_KEY_I				= 0x49,  // I key
		ENUM_KEY_J				= 0x4A,  // J key
		ENUM_KEY_K				= 0x4B,  // K key
		ENUM_KEY_L				= 0x4C,  // L key
		ENUM_KEY_M				= 0x4D,  // M key
		ENUM_KEY_N				= 0x4E,  // N key
		ENUM_KEY_O				= 0x4F,  // O key
		ENUM_KEY_P				= 0x50,  // P key
		ENUM_KEY_Q				= 0x51,  // Q key
		ENUM_KEY_R				= 0x52,  // R key
		ENUM_KEY_S				= 0x53,  // S key
		ENUM_KEY_T				= 0x54,  // T key
		ENUM_KEY_U				= 0x55,  // U key
		ENUM_KEY_V				= 0x56,  // V key
		ENUM_KEY_W				= 0x57,  // W key
		ENUM_KEY_X				= 0x58,  // X key
		ENUM_KEY_Y				= 0x59,  // Y key
		ENUM_KEY_Z				= 0x5A,  // Z key

		ENUM_KEY_NUMPAD0        = 0x60,  // Numeric keypad 0 key
		ENUM_KEY_NUMPAD1        = 0x61,  // Numeric keypad 1 key
		ENUM_KEY_NUMPAD2        = 0x62,  // Numeric keypad 2 key
		ENUM_KEY_NUMPAD3        = 0x63,  // Numeric keypad 3 key
		ENUM_KEY_NUMPAD4        = 0x64,  // Numeric keypad 4 key
		ENUM_KEY_NUMPAD5        = 0x65,  // Numeric keypad 5 key
		ENUM_KEY_NUMPAD6        = 0x66,  // Numeric keypad 6 key
		ENUM_KEY_NUMPAD7        = 0x67,  // Numeric keypad 7 key
		ENUM_KEY_NUMPAD8        = 0x68,  // Numeric keypad 8 key
		ENUM_KEY_NUMPAD9        = 0x69,  // Numeric keypad 9 key

		ENUM_KEY_MULTIPLY       = 0x6A,  // Multiply key
		ENUM_KEY_ADD            = 0x6B,  // Add key
		ENUM_KEY_SEPARATOR      = 0x6C,  // Separator key
		ENUM_KEY_SUBTRACT       = 0x6D,  // Subtract key
		ENUM_KEY_DECIMAL        = 0x6E,  // Decimal key
		ENUM_KEY_DIVIDE         = 0x6F,  // Divide key
		ENUM_KEY_PLUS           = 0xBB,  // Plus Key   (+)
		ENUM_KEY_COMMA          = 0xBC,  // Comma Key  (,)
		ENUM_KEY_MINUS          = 0xBD,  // Minus Key  (-)
		ENUM_KEY_PERIOD         = 0xBE,  // Period Key (.)

		ENUM_KEY_F1             = 0x70,  // F1 key
		ENUM_KEY_F2             = 0x71,  // F2 key
		ENUM_KEY_F3             = 0x72,  // F3 key
		ENUM_KEY_F4             = 0x73,  // F4 key
		ENUM_KEY_F5             = 0x74,  // F5 key
		ENUM_KEY_F6             = 0x75,  // F6 key
		ENUM_KEY_F7             = 0x76,  // F7 key
		ENUM_KEY_F8             = 0x77,  // F8 key
		ENUM_KEY_F9             = 0x78,  // F9 key
		ENUM_KEY_F10            = 0x79,  // F10 key
		ENUM_KEY_F11            = 0x7A,  // F11 key
		ENUM_KEY_F12            = 0x7B,  // F12 key
		ENUM_KEY_COUNT			= 0x62   // this is not a key, but the amount of keycodes there are.
	};
}
}
#endif