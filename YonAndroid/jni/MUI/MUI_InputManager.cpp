#include "MUI_InputManager.h"

namespace mui{

	InputManager::InputManager()
		:m_bInitialized(false),m_pMouseFocusWidget(NULL),m_pMouseFocusLayer(NULL){

	}

	bool InputManager::hasFocusMouseWidget() const{
		return m_pMouseFocusWidget!=NULL;
	}

	bool InputManager::isCaptureMouse() const{
		for (s32 i = MouseButton::Left; i < MouseButton::COUNT; ++i)
		{
			if (m_mouseCaptures[i])
				return true;
		}
		return false;
	}

	void InputManager::initialise(){
		YON_DEBUG_BREAK_IF(m_bInitialized);

		for (s32 i = MouseButton::Left; i < MouseButton::COUNT; ++i)
		{
			m_mouseCaptures[i] = false;
		}

		//TODO 是否需要
		//WidgetManager::getInstance().registerUnlinker(this);
		//Gui::getInstance().eventFrameStart += newDelegate(this, &InputManager::frameEntered);

		m_bInitialized=true;
	}

	void InputManager::shutdown(){
		YON_DEBUG_BREAK_IF(!m_bInitialized);

		//TODO 是否需要
		//Gui::getInstance().eventFrameStart -= newDelegate(this, &InputManager::frameEntered);
		//WidgetManager::getInstance().unregisterUnlinker(this);

		m_bInitialized=false;
	}

	void InputManager::onFrameEntered(u32 time){

	}

	const core::position2di& InputManager::getMousePosition() const{
		return m_mousePosition;
	}

	bool InputManager::injectMouseMove(s32 absx, s32 absy, s32 absz){
		//mMousePosition.set(_absx, _absy);
		m_mousePosition.set(absx,absy);

		//calculate the increase of wheel -- ignore
		//int relz = _absz - mOldAbsZ;
		//mOldAbsZ = _absz;

		/*if (isCaptureMouse())
		{
			if (isFocusMouse())
			{
				if (mLayerMouseFocus != nullptr)
				{
					IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
					for (int i = MouseButton::Button0; i < MouseButton::MAX; ++i)
					{
						if (mMouseCapture[i])
							mWidgetMouseFocus->_riseMouseDrag(point.left, point.top, MouseButton::Enum(i));
					}
				}
			}
			else
			{
				resetMouseCaptureWidget();
			}

			return true;
		}
		Widget* old_mouse_focus = mWidgetMouseFocus;

		// ищем активное окно
		Widget* item = LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);

		// ничего не изменилось
		if (mWidgetMouseFocus == item)
		{
			bool isFocus = isFocusMouse();
			if (isFocusMouse())
			{
				if (mLayerMouseFocus != nullptr)
				{
					IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
					mWidgetMouseFocus->_riseMouseMove(point.left, point.top);
				}
			}
			return isFocus;
		}

		if (item)
		{
			// поднимаемся до рута
			Widget* root = item;
			while (root->getParent()) root = root->getParent();

			// проверяем на модальность
			if (!mVectorModalRootWidget.empty())
			{
				if (root != mVectorModalRootWidget.back())
				{
					item = nullptr;
				}
			}

			if (item != nullptr)
			{
				mLayerMouseFocus = root->getLayer();
			}
		}

		//-------------------------------------------------------------------------------------//
		// новый вид рутового фокуса мыши
		Widget* save_widget = nullptr;

		// спускаемся по новому виджету и устанавливаем рутовый фокус
		Widget* root_focus = item;
		while (root_focus != nullptr)
		{
			if (root_focus->getRootMouseFocus())
			{
				save_widget = root_focus;
				break;
			}

			root_focus->_setRootMouseFocus(true);
			root_focus->_riseMouseChangeRootFocus(true);
			root_focus = root_focus->getParent();
		}

		// спускаемся по старому виджету и сбрасываем фокус
		root_focus = mWidgetMouseFocus;
		while (root_focus != nullptr)
		{
			if (root_focus == save_widget)
				break;

			root_focus->_setRootMouseFocus(false);
			root_focus->_riseMouseChangeRootFocus(false);
			root_focus = root_focus->getParent();
		}
		//-------------------------------------------------------------------------------------//

		// смена фокуса, проверяем на доступность виджета
		if (isFocusMouse() && mWidgetMouseFocus->getEnabled())
		{
			mWidgetMouseFocus->_riseMouseLostFocus(item);
		}

		if ((item != nullptr) && (item->getEnabled()))
		{
			item->_riseMouseMove(_absx, _absy);
			item->_riseMouseSetFocus(mWidgetMouseFocus);
		}

		// запоминаем текущее окно
		mWidgetMouseFocus = item;

		if (old_mouse_focus != mWidgetMouseFocus)
			eventChangeMouseFocus(mWidgetMouseFocus);

		return isFocusMouse();
		*/
		if(isCaptureMouse())
		{
			if(hasFocusMouseWidget())
			{
				if(m_pMouseFocusLayer)
				{
					//因为有m_mousePosition.set(absx,absy);
					const core::position2di& point=m_mousePosition;
					for (s32 i = MouseButton::Left; i < MouseButton::COUNT; ++i)
					{
						if(m_mouseCaptures[i])
							m_pMouseFocusWidget->riseMouseDrag(point.x, point.y, MouseButton::Enum(i));
					}
				}
			}
			else
			{
				resetMouseCaptureWidget();
			}
			return true;
		}

		Widget* oldMouseFocus = m_pMouseFocusWidget;

		//射线拾取到控件item
		//looking for the active window
		Widget* item = LayerManager::getInstance().getWidgetByPosition(absx, absy);

		//Nothing has changed
		if(m_pMouseFocusWidget == item)
		{
			//如果item==NULL,即m_pMouseFocusWidget==NULL,返回false,否则返回true
			//如果item!=NULL且m_pMouseFocusLayer!=NULL,调用m_pMouseFocusWidget->riseMouseMove

			bool focus = hasFocusMouseWidget();
			//if (isFocusMouse())
			if(focus)
			{
				if(m_pMouseFocusLayer != NULL)
				{
					const core::position2di& point = m_mousePosition;
					m_pMouseFocusWidget->riseMouseMove(point.x, point.y);
				}
			}
			return focus;
		}

		if(item)
		{
			//因为modal都是保存根节点的，遍历之；如果匹配，则记录m_pMouseFocusLayer
			//TODO 疑问：为什么move事件也要变动m_pMouseFocusLayer?

			//go up to the root
			Widget* root = item;
			while (root->getParent()) 
				root = root->getParent();

			//check on the modal
			if (!m_modalRootWidgets.empty())
			{
				if (root != m_modalRootWidgets.last())
				{
					item = NULL;
				}
			}

			if (item != NULL)
			{
				m_pMouseFocusLayer = root->getLayer();
			}
		}

		Widget* saveWidget = NULL;

		//get down on the widget and install new focus event
		Widget* rootFocus = item;
		while(rootFocus != NULL)
		{
			if (rootFocus->isRootMouseFocus())
			{
				saveWidget = rootFocus;
				break;
			}

			rootFocus->setRootMouseFocus(true);
			rootFocus->riseMouseChangeRootFocus(true);
			rootFocus = rootFocus->getParent();
		}

		rootFocus = m_pMouseFocusWidget;
		while (rootFocus != NULL)
		{
			if (rootFocus == saveWidget)	//rootFocus==NULL or rootFocus==saveWidget
				break;

			rootFocus->setRootMouseFocus(false);
			rootFocus->riseMouseChangeRootFocus(false);
			rootFocus = rootFocus->getParent();
		}

		//焦点变换通知
		//the change in focus, check the availability of the widget

		if(hasFocusMouseWidget() && m_pMouseFocusWidget->isEnabled())
		{
			m_pMouseFocusWidget->riseMouseLostFocus(item);
		}

		if((item != NULL) && (item->isEnabled()))
		{
			item->riseMouseMove(absx, absy);
			item->riseMouseSetFocus(m_pMouseFocusWidget);
		}

		//remember the current window
		m_pMouseFocusWidget = item;

		if (oldMouseFocus != m_pMouseFocusWidget)
			eventChangeMouseFocus(m_pMouseFocusWidget);

		return hasFocusMouseWidget();	//其实就是item!=NULL


	}

	bool InputManager::injectMousePress(s32 absx, s32 absy, MouseButton mb){
		/*
		injectMouseMove(_absx, _absy, mOldAbsZ);

		Widget* old_key_focus = mWidgetKeyFocus;

		// если мы щелкнули не на гуй
		if (!isFocusMouse())
		{
			resetKeyFocusWidget();

			if (old_key_focus != mWidgetKeyFocus)
				eventChangeKeyFocus(mWidgetKeyFocus);

			return false;
		}

		// если активный элемент заблокирован
		//FIXME
		if (!mWidgetMouseFocus->getEnabled())
			return true;

		if (MouseButton::None != _id && MouseButton::MAX != _id)
		{
			// start capture
			mMouseCapture[_id.getValue()] = true;
			// remember last pressed position
			if (mLayerMouseFocus != nullptr)
			{
				IntPoint point = mLayerMouseFocus->getPosition(_absx, _absy);
				mLastPressed[_id.getValue()] = point;
			}
		}

		// ищем вверх тот виджет который может принимать фокус
		Widget* item = mWidgetMouseFocus;
		while ((item != nullptr) && (!item->getNeedKeyFocus()))
			item = item->getParent();

		// устанавливаем перед вызовом т.к. возможно внутри ктонить поменяет фокус под себя
		setKeyFocusWidget(item);

		if (isFocusMouse())
		{
			mWidgetMouseFocus->_riseMouseButtonPressed(_absx, _absy, _id);

			// после пресса может сброситься
			if (mWidgetMouseFocus)
			{
				// поднимаем виджет, надо подумать что делать если поменялся фокус клавы
				LayerManager::getInstance().upLayerItem(mWidgetMouseFocus);

				// поднимаем пикинг Overlapped окон
				Widget* pick = mWidgetMouseFocus;
				do
				{
					// если оверлаппед, то поднимаем пикинг
					if (pick->getWidgetStyle() == WidgetStyle::Overlapped)
					{
						if (pick->getParent()) pick->getParent()->_forcePick(pick);
					}

					pick = pick->getParent();
				}
				while (pick);
			}
		}

		if (old_key_focus != mWidgetKeyFocus)
			eventChangeKeyFocus(mWidgetKeyFocus);
		*/

		//TODO wheel
		injectMouseMove(absx, absy, 0);

		//TODO key

		//If we do not have clicked on the gui
		if(!hasFocusMouseWidget())
		{
			return false;
		}

		if(!m_pMouseFocusWidget->isEnabled())
			return true;

		if (MouseButton::None != mb && MouseButton::COUNT != mb)
		{
			// start capture
			m_mouseCaptures[mb.getValue()] = true;

			// remember last pressed position
			if (m_pMouseFocusLayer != NULL)
			{
				core::position2di point(absx,absy);
				m_lastPresseds[mb.getValue()] = point;
			}
		}

		//TODO key

		//looking up that widget that can receive focus
		Widget* item = m_pMouseFocusWidget;
		while(item != NULL)
			item = item->getParent();

		if (hasFocusMouseWidget())
		{
			m_pMouseFocusWidget->riseMouseButtonPressed(absx, absy, mb);

			if (m_pMouseFocusWidget)
			{
				LayerManager::getInstance().upLayerItem(m_pMouseFocusWidget);

				//TODO 优化 pick不可能每层都有吧
				//raise picking Overlapped windows
				Widget* pick = m_pMouseFocusWidget;
				do
				{
					// if the overlapped, then the we lift picking
					if (pick->getWidgetStyle() == WidgetStyle::Overlapped)
					{
						if (pick->getParent()) pick->getParent()->front(pick);
					}

					pick = pick->getParent();
				}
				while (pick);
			}
		}

		return true;
	}

	bool InputManager::injectMouseRelease(s32 absx, s32 absy, MouseButton mb){
		/*
		if (isFocusMouse())
		{
			// если активный элемент заблокирован
			if (!mWidgetMouseFocus->getEnabled())
				return true;

			if (_id != MouseButton::None && _id != MouseButton::MAX)
			{
				if (mMouseCapture[_id.getValue()])
				{
					// drop capture
					mMouseCapture[_id.getValue()] = false;
				}
			}

			mWidgetMouseFocus->_riseMouseButtonReleased(_absx, _absy, _id);

			// после вызова, виджет может быть сброшен
			if (nullptr != mWidgetMouseFocus)
			{
				if (MouseButton::Left == _id)
				{
					if (mTimer.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK)
					{
						mWidgetMouseFocus->_riseMouseButtonClick();
						// после вызова, виджет может быть сброшен
						if (nullptr != mWidgetMouseFocus)
							mWidgetMouseFocus->_riseMouseButtonDoubleClick();
					}
					else
					{
						// проверяем над тем ли мы окном сейчас что и были при нажатии
						Widget* item = LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);
						if ( item == mWidgetMouseFocus)
						{
							mWidgetMouseFocus->_riseMouseButtonClick();
						}
						mTimer.reset();
					}
				}
			}

			// для корректного отображения
			injectMouseMove(_absx, _absy, mOldAbsZ);

			return true;
		}
		*/

		if(hasFocusMouseWidget())
		{
			if(!m_pMouseFocusWidget->isEnabled())
				return true;

			if (mb != MouseButton::None && mb != MouseButton::COUNT)
			{
				if (m_mouseCaptures[mb.getValue()])
				{
					// drop capture
					m_mouseCaptures[mb.getValue()] = false;
				}
			}

			m_pMouseFocusWidget->riseMouseButtonReleased(absx, absy, mb);

			//After the call, the widget can be reset
			if (m_pMouseFocusWidget)
			{
				if (MouseButton::Left == mb)
				{
					//if (mTimer.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK)
					//{
					//	mWidgetMouseFocus->_riseMouseButtonClick();
					//	if (nullptr != mWidgetMouseFocus)
					//		mWidgetMouseFocus->_riseMouseButtonDoubleClick();
					//}
					//else
					{
						Widget* item = LayerManager::getInstance().getWidgetByPosition(absx, absy);
						if ( item == m_pMouseFocusWidget)
						{
							m_pMouseFocusWidget->riseMouseButtonClick();
						}
						//mTimer.reset();
					}
				}
			}
		}
	}

	void InputManager::addModalWidget(Widget* widget){
		YON_DEBUG_BREAK_IF(widget==NULL||widget->getParent()!=NULL);

		//_resetMouseFocusWidget();
		//removeWidgetModal(_widget);
		//mVectorModalRootWidget.push_back(_widget);

		//setKeyFocusWidget(_widget);
		//LayerManager::getInstance().upLayerItem(_widget);

		//TODO
		resetMouseFocusWidget();
		removeModalWidget(widget);
		m_modalRootWidgets.push_back(widget);
	}

	void InputManager::removeModalWidget(Widget* widget){
		
		//resetKeyFocusWidget(_widget);
		//_resetMouseFocusWidget();

		//for (VectorWidgetPtr::iterator iter = mVectorModalRootWidget.begin(); iter != mVectorModalRootWidget.end(); ++iter)
		//{
		//	if (*iter == _widget)
		//	{
		//		mVectorModalRootWidget.erase(iter);
		//		break;
		//	}
		//}
		//if (!mVectorModalRootWidget.empty())
		//{
		//	setKeyFocusWidget(mVectorModalRootWidget.back());
		//	LayerManager::getInstance().upLayerItem(mVectorModalRootWidget.back());
		//}

		for(u32 i=0;i<m_modalRootWidgets.size();++i)
		{
			if(m_modalRootWidgets[i]==widget)
			{
				m_modalRootWidgets.erase(i);
				break;
			}
		}
		if(m_modalRootWidgets.empty()==false)
		{
			//TODO
		}
	}

	bool InputManager::hasModalWidget() const{
		return m_modalRootWidgets.empty()==false;
	}

	void InputManager::resetMouseCaptureWidget(){
		for (s32 i = MouseButton::Left; i < MouseButton::COUNT; ++i)
		{
			m_mouseCaptures[i] = false;
		}
	}

	void InputManager::resetMouseFocusWidget(){
		/*
		Widget* mouseFocus = mWidgetMouseFocus;
		mWidgetMouseFocus = nullptr;

		// спускаемся по старому виджету и сбрасываем фокус
		Widget* root_focus = mouseFocus;
		while (root_focus != nullptr)
		{
			root_focus->_setRootMouseFocus(false);
			root_focus->_riseMouseChangeRootFocus(false);
			root_focus = root_focus->getParent();
		}

		for (int i = MouseButton::Button0; i < MouseButton::MAX; ++i)
		{
			if (mMouseCapture[i])
			{
				mMouseCapture[i] = false;
				mouseFocus->_riseMouseButtonReleased(mLastPressed[i].left, mLastPressed[i].top, MouseButton::Enum(i));
			}
		}

		if (nullptr != mouseFocus)
		{
			mouseFocus->_riseMouseLostFocus(nullptr);
		}
		*/

		Widget* mouseFocus = m_pMouseFocusWidget;
		m_pMouseFocusWidget = NULL;

		//down the old widget and reset the focus
		Widget* parent = mouseFocus;
		while(parent != NULL)
		{
			parent->setRootMouseFocus(false);
			parent->riseMouseChangeRootFocus(false);
			parent = parent->getParent();
		}

		for (s32 i = MouseButton::Left; i < MouseButton::COUNT; ++i)
		{
			if(m_mouseCaptures[i])
			{
				m_mouseCaptures[i] = false;
				mouseFocus->riseMouseButtonReleased(m_lastPresseds[i].x, m_lastPresseds[i].y, MouseButton::Enum(i));
			}
		}

		if (mouseFocus)
		{
			mouseFocus->riseMouseLostFocus(NULL);
		}
	}

}