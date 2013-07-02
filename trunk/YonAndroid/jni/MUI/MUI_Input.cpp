#include "MUI_Input.h"

namespace mui{

	void Input::riseMouseLostFocus(Widget* widget){
		onMouseLostFocus(widget);
		eventMouseLostFocus(static_cast<Widget*>(this), widget);
	}
	void Input::riseMouseSetFocus(Widget* widget){
		onMouseSetFocus(widget);
		eventMouseSetFocus(static_cast<Widget*>(this), widget);
	}
	void Input::riseMouseDrag(s32 left, s32 top, MouseButton mb){
		onMouseDrag(left, top, mb);
		eventMouseDrag(static_cast<Widget*>(this), left, top, mb);
	}
	void Input::riseMouseMove(s32 left, s32 top){
		onMouseMove(left, top);
		eventMouseMove(static_cast<Widget*>(this), left, top);
	}
	void Input::riseMouseButtonPressed(s32 left, s32 top, MouseButton mb){
		onMouseButtonPressed(left, top, mb);
		eventMouseButtonPressed(static_cast<Widget*>(this), left, top, mb);
	}
	void Input::riseMouseButtonReleased(s32 left, s32 top, MouseButton mb){
		onMouseButtonReleased(left, top, mb);
		eventMouseButtonReleased(static_cast<Widget*>(this), left, top, mb);
	}
	void Input::riseMouseButtonClick(){
		onMouseButtonClick();
		eventMouseButtonClick(static_cast<Widget*>(this));
	}
	void Input::riseMouseChangeRootFocus(bool focus){
		onMouseChangeRootFocus(focus);
		eventRootMouseChangeFocus(static_cast<Widget*>(this), focus);
	}

	void Input::setRootMouseFocus(bool value){
		m_bRootMouseFocus=value;
	}

	//空实现，子类必要时提供实现
	void Input::onMouseLostFocus(Widget* widget){
	}
	void Input::onMouseSetFocus(Widget* widget){
	}
	void Input::onMouseDrag(s32 left, s32 top, MouseButton mb){
	}
	void Input::onMouseMove(s32 left, s32 top){
	}
	void Input::onMouseButtonPressed(s32 left, s32 top, MouseButton mb){
	}
	void Input::onMouseButtonReleased(s32 left, s32 top, MouseButton mb){
	}
	void Input::onMouseButtonClick(){
	}
	void Input::onMouseChangeRootFocus(bool focus){
	}

	Input::Input(){
	}
	Input::~Input(){
	}

	void Input::setNeedToolTip(bool value){
		m_bNeedToolTip=value;
	}

	bool Input::needToolTip() const{
		return m_bNeedToolTip;
	}

	void Input::setNeedMouseFocus(bool value){
		m_bNeedMouseFocus=value;
	}

	bool Input::needMouseFocus() const{
		return m_bNeedMouseFocus;
	}

	bool Input::isRootMouseFocus() const{
		return m_bRootMouseFocus;
	}
}