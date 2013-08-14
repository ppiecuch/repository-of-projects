#ifndef _MUI_WIDGETMANAGER_H_
#define _MUI_WIDGETMANAGER_H_

namespace mui{

	class WidgetManager{
	public:
		WidgetManager();

		void initialise();
		void shutdown();

		Widget* createWidget(WidgetStyle style, const core::stringc& type, const core::stringc& skin, const core::coordi& coord, Widget* parent, ICroppedRectangle* cropeedParent, const core::stringc& name);
	};
}
#endif