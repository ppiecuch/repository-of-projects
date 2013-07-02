#ifndef _MUI_BUTTON_H_
#define _MUI_BUTTON_H_

namespace mui{

	class Button : public Label{
	private:

		//bool mIsMousePressed;
		bool m_bPressed;

		//bool mIsMouseFocus;
		bool m_bFocused;

		//Status button is pressed or not
		//bool mStateSelected;
		bool m_bStateSelected;

		void updateButtonState();

		//void _setMouseFocus(bool _focus);
		void setMouseFocus(bool on);

	protected:

		void setStateSelected(bool on);

		//virtual void onMouseLostFocus(Widget* _new);
		//virtual void onMouseSetFocus(Widget* _old);
		virtual void onMouseBlur(Widget* w);
		virtual void onMouseFocus(Widget* w);

		//virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		//virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMousePressed(const core::position2di& pos, MouseButton mb);
		virtual void onMouseReleased(const core::position2di& pos, MouseButton mb);

	public:

		Button();

		// Get buton selected
		//bool getStateSelected() const;
		bool isStateSelected() const;
	};
}
#endif