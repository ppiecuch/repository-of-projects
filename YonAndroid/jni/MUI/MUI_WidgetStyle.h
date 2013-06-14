#ifndef _MUI_WIDGETSTYLE_H_
#define _MUI_WIDGETSTYLE_H_

namespace mui{

	struct WidgetStyle{
	private:
		Enum value;
	public:
		//子级/弹出/重叠
		//对于Window下的窗口，有以下特性：
		//子窗口与重叠窗口或弹出窗口的主要区别是子窗口只能出现在另一个窗口中，并且子窗口的任何多余部分都被该窗口移去或剪切掉。
		//重叠窗口和弹出窗口的主要区别是弹出窗口出现时可以没有标题(也称为标题栏)。
		//弹出窗口通常以对话框和消息框的形式与用户对话。
		//重叠窗口通常用于建立应用程序主窗口。事实上，有时也叫做“主”窗口或者“框架”窗口。
		//子窗口通常用在视图(View)中，如在文本编辑器中的文本显示，也用在控件中，如在对话框中的OK按钮。而对那些看起来像按钮或控件的子窗口，也称为“控件”窗口。
		enum Enum
		{
			Child, /**< child widget, cropped by parent widget borders, no overlapping (used by default for child widgets) */
			Popup, /**< popup widget, have parent widget, but not cropped on its borders */
			Overlapped, /**< child widget, cropped by parent widget borders, can overlap (used by default for root widgets) */
			COUNT
		};

		WidgetStyle() : 
			value(COUNT){
		}

		WidgetStyle(Enum value) :
			value(value){
		}

		friend bool operator == (const WidgetStyle& a, const WidgetStyle& b)
		{
			return a.value == b.value;
		}

		friend bool operator != (const WidgetStyle& a, const WidgetStyle& b)
		{
			return a.value != b.value;
		}

		const char* getValueName() const
		{
			static const char* values[COUNT + 1] = { "Child", "Popup", "Overlapped", "" };
			return values[(value < COUNT && value >= Child) ? value : COUNT];
		}
	};
}
#endif