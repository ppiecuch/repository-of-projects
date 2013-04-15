#ifndef _YON_IHOOK_H_
#define _YON_IHOOK_H_

#include "yonConfig.h"

#ifdef YON_COMPILE_WITH_WIN32
#include "IReferencable.h"
#include <Windows.h>

namespace yon{

	typedef LRESULT (*MouseHookCallback)(int nCode,WPARAM wparam,LPARAM lparam);

	class IHook : public core::IReferencable{
	public:
		virtual bool hook(HWND hWnd) = 0;
		virtual bool unhook() = 0;
		
	};

	extern "C" YON_API IHook* createHook(MouseHookCallback func);
}
#endif
#endif