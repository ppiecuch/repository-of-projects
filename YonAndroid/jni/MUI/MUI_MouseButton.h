#ifndef _MUI_MOUSEBUTTON_H_
#define _MUI_MOUSEBUTTON_H_

namespace mui{

	//MouseButton
	struct MouseButton{
		enum Enum
		{
			None = -1,
			Left = 0,
			Middle,
			Right,

			//Button0 = 0,
			//Button1,
			//Button2,
			//Button3,
			//Button4,
			//Button5,
			//Button6,
			//Button7,
			COUNT
		};

		MouseButton(Enum value = None) 
			:m_value(value)
		{
		}

		friend bool operator == (MouseButton const& a, MouseButton const& b)
		{
			return a.m_value == b.m_value;
		}

		friend bool operator != (MouseButton const& a, MouseButton const& b)
		{
			return a.m_value != b.m_value;
		}

		Enum getValue() const
		{
			return m_value;
		}

	private:
		Enum m_value;
	};
}
#endif