#ifndef _MUI_ALIGN_H_
#define _MUI_ALIGN_H_

namespace mui{

	struct Align{
	private:
		Enum value;
	public:
		enum Enum{
			HCenter = 0, //center horizontally
			VCenter = 0, //center vertically
			Center = HCenter | VCenter, //center in the dead center

			Left = 0x2, //value from the left (and center vertically)
			Right = 0x4, //value from the right (and center vertically)
			HStretch = Left | Right, //stretch horizontally proportionate to parent window (and center vertically)

			Top = 0x8, //value from the top (and center horizontally)
			Bottom = 0x10, //value from the bottom (and center horizontally)
			VStretch = Top | Bottom, //stretch vertically proportionate to parent window (and center horizontally)

			Stretch = HStretch | VStretch, //stretch proportionate to parent window
			Default = Left | Top, //default value (value from left and top)

			COUNT = 11
		};
		//缺点是水平方向跟垂直方向没有默认值
		/*enum Enum{
			HCenter = 0x1, //center horizontally
			VCenter = 0x2, //center vertically
			Center = HCenter | VCenter, //center in the dead center

			Left = 0x4, //value from the left (and center vertically)
			Right = 0x8, //value from the right (and center vertically)
			HStretch = Left | Right, //stretch horizontally proportionate to parent window (and center vertically)

			Top = 0x10, //value from the top (and center horizontally)
			Bottom = 0x20, //value from the bottom (and center horizontally)
			VStretch = Top | Bottom, //stretch vertically proportionate to parent window (and center horizontally)

			Stretch = HStretch | VStretch, //stretch proportionate to parent window
			Default = Left | Top //default value (value from left and top)
		};*/

		Align(Enum value = Default) :
			value(value)
		{
		}

		bool isHCenter() const
		{
			return HCenter == (value & HStretch);
			//return value&HCenter;
		}

		bool isVCenter() const
		{
			return VCenter == (value & VStretch);
			//return value&VCenter;
		}

		bool isCenter() const
		{
			return Center == (value & Stretch);
			//return value&Center;
		}

		bool isLeft() const
		{
			return Left == (value & HStretch);
			//return value&Left;
		}

		bool isRight() const
		{
			return Right == (value & HStretch);
			//return value&Right;
		}

		bool isHStretch() const
		{
			return HStretch == (value & HStretch);
			//return value&HStretch;
		}

		bool isTop() const
		{
			return Top == (value & VStretch);
			//return value&Top;
		}

		bool isBottom() const
		{
			return Bottom == (value & VStretch);
			//return value&Bottom;
		}

		bool isVStretch() const
		{
			return VStretch == (value & VStretch);
			//return value&VStretch;
		}

		bool isStretch() const
		{
			return Stretch == (value & Stretch);
			//return value&Stretch;
		}

		bool isDefault() const
		{
			return Default == (value & Stretch);
			//return value&Default;
		}

		Align& operator |= (const Align& other)
		{
			value = Enum(value | other.value);
			return *this;
		}

		friend Align operator | (Enum a, Enum b)
		{
			return Align(Enum(a | b));
		}

		friend Align operator | (const Align& a, const Align& b)
		{
			return Align(Enum(a.value | b.value));
		}

		friend bool operator == (const Align& a, const Align& b)
		{
			return a.value == b.value;
		}

		friend bool operator != (const Align& a, const Align& b)
		{
			return a.value != b.value;
		}

		const char* getValueName() const
		{
			YON_DEBUG_BREAK_IF(value<HCenter||value>=COUNT);
			static const char* values[COUNT] = { "HCenter", "VCenter", "Center", "Left", "Right", "HStretch", "Top", "Bottom", "VStretch", "Stretch", "Default" };
			return values[value];
		}
	};
}
#endif