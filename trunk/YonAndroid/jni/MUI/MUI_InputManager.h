#ifndef _MUI_INPUTMANAGER_H_
#define _MUI_INPUTMANAGER_H_

namespace mui{

	//TODO ��Ϊֻ���������¼���ҵ���¼�����GUI���ִ���
	//����Focus�Ĵ���
	//��ȻFocus����ʾ������Ҫ�����ǳ����ȶ��Լ���չ�ԵĿ��ǣ�����ҵ��ģ�������Focus�Ĵ���,��������ʾ����Ĵ���
	class InputManager{
	private:

		//Widget* mWidgetMouseFocus;
		//ILayer* mLayerMouseFocus;
		Widget* m_pMouseFocusWidget;
		ILayer* m_pMouseFocusLayer;

		//IntPoint mMousePosition;
		core::position2di m_mousePosition;

		// last mouse press position
		//IntPoint mLastPressed[MouseButton::MAX];
		core::position2di m_lastPresseds[MouseButton::COUNT];

		// is mouse button captured by active widget
		//bool mMouseCapture[MouseButton::MAX];
		bool m_mouseCaptures[MouseButton::COUNT];

		//list of widgets with modal mode
		//VectorWidgetPtr mVectorModalRootWidget;
		core::array<Widget*> m_modalRootWidgets;

		//bool mIsInitialise;
		bool m_bInitialized;

		//void frameEntered(float _frame);
		void onFrame(u32 time);

		void resetMouseFocusWidget();

		friend class Widget;
	public:
		InputManager();

		void initialize();
		void shutdown();

		/** 
		 *@brief Get current mouse position on screen 
		 */
		//const IntPoint& getMousePosition() const;
		const core::position2di& getMousePosition() const;

		/** 
		 *@brief Get mouse position on current layer.
		 
		 *This position might different from getMousePosition() if mouse is over non-2d layer.
		 */
		//��������Ϊ�ڲ�ʹ�õ� mLayerMouseFocus->getPositionҲ������
		//ֱ��ʹ��getMousePosition����
		//IntPoint getMousePositionByLayer();

		/**
		 *@brief Is any widget have mouse focus 
		 */
		//bool isFocusMouse() const;
		bool hasFocusMouseWidget() const;

		/**
		 *@brief Is any widget captured mouse 
		 */
		bool isCaptureMouse() const;

		/** 
		 *@brief Inject MouseMove event
		 *@return true if event has been processed by GUI
		 */
		bool injectMouseMove(s32 absx, s32 absy, s32 absz);

		/** 
		 *@brief Inject MousePress event
		 *@return true if event has been processed by GUI
		 */
		bool injectMousePress(s32 absx, s32 absy, MouseButton mb);

		/** 
		 *@brief Inject MouseRelease event
		 *@return true if event has been processed by GUI
		 */
		bool injectMouseRelease(s32 absx, s32 absy, MouseButton mb);

		//TODO Multitouch

		/** 
		 *@brief Add modal widget
		 *
		 *all other widgets inaccessible while modal widget exist 
		 */
		//void addWidgetModal(Widget* widget);
		void addModalWidget(Widget* widget);

		/** 
		 * @brief Remove modal widget 
		 */
		//void removeWidgetModal(Widget* widget);
		void removeModalWidget(Widget* widget);

		/** 
		* @brief Return true if any modal widget exist 
		*/
		//bool isModalAny() const;
		bool hasModalWidget() const;

		/** 
		 *@brief Reset mouse capture.
		 *For example when we dragging and application
		 *lost focus you should call this.
		 */
		void resetMouseCaptureWidget();

		/** 
		 *@brief Event : MultiDelegate. Mouse focus was changed.\n
		 *signature : void method(MyGUI::Widget* widget)\n
		 *@param widget
		 */
		//delegates::CMultiDelegate1<Widget*> eventChangeMouseFocus;
		delegates::CMultiDelegate1<Widget*> focusChangeEvents;
	};
}
#endif