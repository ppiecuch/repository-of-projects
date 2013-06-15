#ifndef _MUI_ISHAPWIDGET_H_
#define _MUI_ISHAPWIDGET_H_

namespace mui{

	//ISubWidgetRect
	class IShapWidget : IBaseWidget{
	protected:
		virtual void setUVRect(const core::rectf& uv) { }
		virtual void setColor(const video::SColor& value) { }
	public:
		virtual ~IShapWidget() {}

		//virtual void _setUVSet(const FloatRect& _rect) { }
		//virtual void _setColour(const Colour& _value) { }
	};
}
#endif