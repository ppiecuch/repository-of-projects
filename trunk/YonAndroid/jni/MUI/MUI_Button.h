#ifndef _MUI_BUTTON_H_
#define _MUI_BUTTON_H_

namespace mui{

	class Button : public Label{
	private:

		//bool mIsMousePressed;
		bool m_bPressed;

		//void updateButtonState();
		void update();

	protected:

		//virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		//virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMousePressed(const core::position2di& pos, MouseButton mb);
		virtual void onMouseReleased(const core::position2di& pos, MouseButton mb);

	public:

		Button();
	};
}
#endif