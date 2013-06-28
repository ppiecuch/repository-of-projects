#ifndef _MUI_INPUTMANAGER_H_
#define _MUI_INPUTMANAGER_H_

namespace mui{

	class InputManager{
	public:
		InputManager();

		void initialise();
		void shutdown();

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
	};
}
#endif