#ifndef _MUI_INPUT_H_
#define _MUI_INPUT_H_

#include "MUI_Prepare.h"
#include "MUI_MouseButton.h"
#include "MUI_EventPair.h"

namespace mui{

	typedef core::CMultiDelegate1<void,Widget*> EventHandle_WidgetVoid;
	typedef core::CMultiDelegate2<void,Widget*, Widget*> EventHandle_WidgetWidget;
	typedef core::CMultiDelegate2<void,Widget*, bool> EventHandle_WidgetBool;
	typedef core::CMultiDelegate2<void,Widget*, int> EventHandle_WidgetInt;
	typedef core::CMultiDelegate2<void,Widget*, size_t> EventHandle_WidgetSizeT;
	typedef core::CMultiDelegate3<void,Widget*, int, int> EventHandle_WidgetIntInt;
	typedef core::CMultiDelegate4<void,Widget*, int, int, MouseButton> EventHandle_WidgetIntIntButton;

	//只被Widget继承
	//WidgetInput
	class Input{
	private:
		//std::string mPointer;
		//MaskPickInfo mOwnMaskPickInfo;

		//bool mNeedToolTip;
		bool m_bNeedToolTip;

		//bool mInheritsPick;
		//bool mNeedKeyFocus;

		//bool mNeedMouseFocus;
		bool m_bNeedMouseFocus;

		//bool mRootMouseFocus;
		bool m_bRootMouseFocus;

		//bool mRootKeyFocus;

		//只被InputManager 调用
		void riseMouseLostFocus(Widget* widget);
		void riseMouseSetFocus(Widget* widget);
		void riseMouseDrag(s32 left, s32 top, MouseButton mb);
		void riseMouseMove(s32 left, s32 top);
		//void riseMouseWheel(s32 rel);
		void riseMouseButtonPressed(s32 left, s32 top, MouseButton mb);
		void riseMouseButtonReleased(s32 left, s32 top, MouseButton mb);
		void riseMouseButtonClick();
		//void riseMouseButtonDoubleClick();
		//void riseKeyLostFocus(Widget* new);
		//void riseKeySetFocus(Widget* old);
		//void riseKeyButtonPressed(KeyCode key, Char char);
		//void riseKeyButtonReleased(KeyCode key);
		void riseMouseChangeRootFocus(bool focus);
		//void riseKeyChangeRootFocus(bool focus);

		void setRootMouseFocus(bool value);
		//void setRootKeyFocus(bool value);
	protected:
		virtual void onMouseLostFocus(Widget* widget);
		virtual void onMouseSetFocus(Widget* widget);
		virtual void onMouseDrag(s32 left, s32 top, MouseButton mb);
		virtual void onMouseMove(s32 left, s32 top);
		//virtual void onMouseWheel(s32 rel);
		virtual void onMouseButtonPressed(s32 left, s32 top, MouseButton mb);
		virtual void onMouseButtonReleased(s32 left, s32 top, MouseButton mb);
		virtual void onMouseButtonClick();
		//virtual void onMouseButtonDoubleClick();
		virtual void onMouseChangeRootFocus(bool focus);
	public:
		Input();
		virtual ~Input();

		/** 
		 *@brief Set need tool tip mode flag. Enable this if you need tool tip events for widget 
		 */
		void setNeedToolTip(bool value);

		/** 
		 *@brief Get need tool tip mode flag 
		 */
		//bool getNeedToolTip() const;
		bool needToolTip() const;

		/** 
		 *@brief Set need mouse focus flag 
		 */
		void setNeedMouseFocus(bool value);

		/** 
		 *@brief Is need mouse focus

		 *If disable this widget won't be reacting on mouse at all.\n
		 *Enabled (true) by default.
		 */
		//bool getNeedMouseFocus() const;
		bool needMouseFocus() const;

		/** 
		 *@brief Set need key focus flag 
		 */
		//void setNeedKeyFocus(bool value);

		/** 
		 *@brief Is need key focus
		 *If disable this widget won't be reacting on keyboard at all.\n
		 *Enabled (true) by default.
		 */
		//bool getNeedKeyFocus() const;

		/** 
		 *@brief Set inherits mode flag
		 *This mode makes all child widgets pickable even if widget don't
		 *need mouse focus (was set setNeedKeyFocus(false) ).\n
		 *Disabled (false) by default.
		 */
		//void setInheritsPick(bool _value);

		/** 
		 *@brief Get inherits mode flag
		 */
		//bool getInheritsPick() const;

		/** 
		 *@brief Set picking mask for widget 
		 */
		//void setMaskPick(const std::string& _filename);

		/** 
		 *@brief Set picking mask for widget 
		 */
		//void setMaskPick(const MaskPickInfo& _info);

		//bool isMaskPickInside(const IntPoint& _point, const IntCoord& _coord) const;

		bool getRootMouseFocus() const;
		bool isRootMouseFocus() const;
		//bool getRootKeyFocus() const;


		/** 
		 *@brief Event : Widget lost mouse focus.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
		 *@param _sender widget that called this event
		 *@param _new widget with mouse focus or nullptr
		 */
		EventHandle_WidgetWidget eventMouseLostFocus;

		/** 
		 *@brief Event : Widget got mouse focus.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
		 *@param _sender widget that called this event
		 *@param _old widget with mouse focus or nullptr
		 */
		EventHandle_WidgetWidget eventMouseSetFocus;

		/** 
		 *@brief Event : Widget mouse move with captured widget.\n
		 *signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		 *@param _sender widget that called this event
		 *@param _left - pointer position
		 *@param _top - pointer position
		 */
		EventPairAddParameter<EventHandle_WidgetIntInt, EventHandle_WidgetIntIntButton> eventMouseDrag;

		/** 
		 *@brief Event : Mouse move over widget.\n
		 *signature : void method(MyGUI::Widget* _sender, int _left, int _top)\n
		 *@param _sender widget that called this event
		 *@param _left - pointer position
		 *@param _top - pointer position
		 */
		EventHandle_WidgetIntInt eventMouseMove;

		/** 
		 *@brief Event : Mouse wheel over widget.\n
		 *signature : void method(MyGUI::Widget* _sender, int _rel)\n
		 *@param _sender widget that called this event
		 *@param _rel relative wheel position
		 */
		//EventHandle_WidgetInt eventMouseWheel;

		/** 
		 *@brief Event : Mouse button pressed.\n
		 *signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		 *@param _sender widget that called this event
		 *@param _left - pointer position
		 *@param _top - pointer position
		 *@param _id Mouse button id
		 */
		EventHandle_WidgetIntIntButton eventMouseButtonPressed;

		/** 
		 *@brief Event : Mouse button released.\n
		 *signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		 *@param _sender widget that called this event
		 *@param _left - pointer position
		 *@param _top - pointer position
		 *@param _id Mouse button id
		 */
		EventHandle_WidgetIntIntButton eventMouseButtonReleased;

		//TODO 听说原版只有当Pos(press)==Pos(release)时才算是click事件
		/** 
		 *Event : Mouse button pressed and released.\n
		 *signature : void method(MyGUI::Widget* _sender)
		 *@param _sender widget that called this event
		 */
		EventHandle_WidgetVoid eventMouseButtonClick;

		/** 
		 *@brief Event : Mouse button double click.\n
		 *signature : void method(MyGUI::Widget* _sender)
		 *@param _sender widget that called this event
		 */
		//EventHandle_WidgetVoid eventMouseButtonDoubleClick;

		/** 
		 *@brief Event : Widget lost keyboard focus.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
		 *@param _sender widget that called this event
		 *@param _new widget with keyboard focus or nullptr
		 */
		//EventHandle_WidgetWidget eventKeyLostFocus;

		/** 
		 *@brief Event : Widget got keyboard focus.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
		 *@param _sender widget that called this event
		 *@param _old widget with keyboard focus or nullptr
		 */
		//EventHandle_WidgetWidget eventKeySetFocus;

		/** 
		 *@brief Event : Key pressed.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
		 *@param _sender widget that called this event
		 *@param _key code
		 *@param _char of pressed symbol (for multilanguage applications)
		 */
		//EventHandle_WidgetKeyCodeChar eventKeyButtonPressed;

		/** 
		 *@brief Event : Key released.\n
		 *signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key)\n
		 *@param _sender widget that called this event
		 *@param _key code
		 */
		//EventHandle_WidgetKeyCode eventKeyButtonReleased;

		/** 
		 *@brief Event : Root widget changed mouse focus.\n
		 *info : this event sends only to root widget\n
		 *signature : void method(MyGUI::Widget* _sender, bool _focus);
		 *@param _sender widget that called this event
		 *@param _focus Is widget got mouse focus.
		 */
		EventHandle_WidgetBool eventRootMouseChangeFocus;

		/** 
		 *@brief Event : Root widget changed keyboard focus.\n
		 *info : this event sends only to root widget\n
		 *signature : void method(MyGUI::Widget* _sender, bool _focus);
		 *@param _sender widget that called this event
		 *@param _focus Is widget got keyboard focus.
		 */
		//EventHandle_WidgetBool eventRootKeyChangeFocus;

		/** 
		 *@brief Event : Event about changing tooltip state.\n
		 *signature : void method(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		 *@param _sender widget that called this event
		 *@param _info about tooltip
		 */
		//EventHandle_WidgetToolTip eventToolTip;

		//rise***部分需要由InputManager调用
		friend class InputManager;
	};
}
#endif