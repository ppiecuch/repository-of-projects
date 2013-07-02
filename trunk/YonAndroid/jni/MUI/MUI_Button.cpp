#include "MUI_Button.h"

namespace mui{

	Button::Button()
		:m_bPressed(false),m_bFocused(false),m_bStateSelected(false){

	}

	void Button::updateButtonState(){
		/*
		if (mStateSelected)
		{
			if (!getEnabled())
			{
				if (!_setState("disabled_checked"))
					_setState("disabled");
			}
			else if (mIsMousePressed)
			{
				if (!_setState("pushed_checked"))
					_setState("pushed");
			}
			else if (mIsMouseFocus)
			{
				if (!_setState("highlighted_checked"))
					_setState("pushed");
			}
			else
				_setState("normal_checked");
		}
		else
		{
			if (!getEnabled())
				_setState("disabled");
			else if (mIsMousePressed)
				_setState("pushed");
			else if (mIsMouseFocus)
				_setState("highlighted");
			else
				_setState("normal");
		}
		*/
		
		//TODO
	}

	void Button::setMouseFocus(bool on){
		m_bFocused = on;
		updateButtonState();
	}

	void Button::setStateSelected(bool on)
	{
		if (m_bStateSelected == on)
			return;

		m_bStateSelected = on;
		updateButtonState();
	}

	void Button::onMouseBlur(Widget* w){
		//_setMouseFocus(false);

		//Base::onMouseLostFocus(_new);

		setMouseFocus(false);
		Label::onMouseSetFocus(w);
	}

	void Button::onMouseFocus(Widget* w){
		//_setMouseFocus(true);

		//Base::onMouseSetFocus(_old);

		setMouseFocus(true);
		Label::onMouseSetFocus(w);
	}

	void Button::onMousePressed(const core::position2di& pos, MouseButton mb){
		/*
		if (_id == MouseButton::Left)
		{
			mIsMousePressed = true;
			updateButtonState();
		}

		Base::onMouseButtonPressed(_left, _top, _id);
		*/

		if(mb==MouseButton::Left)
		{
			m_bPressed=true;
			updateButtonState();
		}
		Label::onMouseButtonPressed(pos,mb);
	}

	void Button::onMouseReleased(const core::position2di& pos, MouseButton mb){
		/*
		if (_id == MouseButton::Left)
		{
			mIsMousePressed = false;
			updateButtonState();
		}

		Base::onMouseButtonReleased(_left, _top, _id);
		*/
		if(mb==MouseButton::Left)
		{
			m_bPressed=false;
			updateButtonState();
		}
		Label::onMouseButtonReleased(pos,mb);
	}

	bool Button::isStateSelected() const{
		return m_bStateSelected;
	}
}