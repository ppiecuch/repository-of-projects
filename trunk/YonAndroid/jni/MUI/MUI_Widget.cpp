#include "MUI_Widget.h"

namespace mui{

	void Widget::front(Widget* widget){
		YON_DEBUG_BREAK_IF(m_pWidgetClient==this);

		if(m_pWidgetClient)
			m_pWidgetClient->tail(widget);

		s32 index=m_children.binary_search(widget);

		if(index!=-1)
		{
			m_children.erase(index);
			m_children.push_back(widget);
		}
	}
}