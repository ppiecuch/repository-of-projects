#ifndef _MUI_CONTROLLERITEM_H_
#define _MUI_CONTROLLERITEM_H_

namespace mui{

	typedef core::CMultiDelegate1<Widget*> EventHandle_WidgetPtr;
	typedef core::CMultiDelegate2<Widget*, ControllerItem*> EventHandle_WidgetPtrControllerItemPtr;

	class ControllerItem{
	public:
		virtual ~ControllerItem() { }

		virtual void prepareItem(Widget* widget) = 0;
		virtual bool addTime(Widget* widget, f32 time) = 0;

		virtual void setProperty(const core::stringc& key, const core::stringc& value) {}

		/** 
		* @brief Event : Before controller started working.
		*
		* signature : void method(Widget* _sender, ControllerItem* _controller)\n
		* @param _sender widget under control
		*/
		EventPairAddParameter<EventHandle_WidgetPtr, EventHandle_WidgetPtrControllerItemPtr> eventPreAction;

		/** 
		* @brief Event : Controller updated (called every frame).
		*
		* signature : void method(Widget* _sender, ControllerItem* _controller)
		* @param _sender widget under control
		*/
		EventPairAddParameter<EventHandle_WidgetPtr, EventHandle_WidgetPtrControllerItemPtr> eventUpdateAction;

		/** 
		* @brief Event : After controller finished working.
		*
		* signature : void method(Widget* _sender, ControllerItem* _controller)
		* @param _sender widget under control
		*/
		EventPairAddParameter<EventHandle_WidgetPtr, EventHandle_WidgetPtrControllerItemPtr> eventPostAction;
	};
}
#endif