#include "MUI_Widget.h"

namespace mui{

	void Widget::front(Widget* widget){
		YON_DEBUG_BREAK_IF(m_pWidgetClient==this);

		if(m_pWidgetClient)
			m_pWidgetClient->tail(widget);

		s32 index=m_widgetChildren.binary_search(widget);

		if(index!=-1)
		{
			m_widgetChildren.erase(index);
			m_widgetChildren.push_back(widget);
		}
	}
}