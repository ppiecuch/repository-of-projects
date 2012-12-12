#include "stdio.h"
#include<windows.h>

#include <crtdbg.h>
#include <typeinfo>

//#include <vld.h>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#ifdef YON_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//YON_HIDE_CONSOLE

#include "framework.h"
#include "resource.h"
INT_PTR CALLBACK DlgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			SetDlgItemText(hWnd, IDC_EDIT1, L"Please enter the txt");
			// Set focus back to the edit control
			//SendDlgItemMessage(hWnd, IDC_EDIT1, WM_SETFOCUS, 0, 0);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				int len = GetWindowTextLengthA(GetDlgItem(hWnd,IDC_EDIT1));
				if(len > 0){
					char *buff = new char[len+1];
					GetDlgItemTextA(hWnd, IDC_EDIT1, buff, len+1);
					Logger->debug("%s\n",buff);
					delete buff;
				}
				EndDialog(hWnd, 0);
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}
		break;
	}

	return FALSE;
}  

class MyCallback : public platform::ICallback{
public:
	virtual bool callback(const platform::SCallback& cb)
	{
		switch(cb.type)
		{
		case ENUM_CALLBACK_TYPE_UI:
			{
				switch(cb.ui.type)
				{
				case ENUM_CALLBACK_UI_TYPE_EDITBOX:
					DialogBox(NULL,MAKEINTRESOURCE(IDD_DIALOG1),(HWND)getEngine()->getCreateParameters().windowId,DlgWndProc);
					return true;
				}
			}
		default:
			Logger->warn("unexpect callback ui type\r\n");
		}
		return false;
	}
};
ICallback* cb=NULL;
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	cb=new MyCallback();
	init(NULL,cb,NULL,NULL,800,480);

	while(getEngine()->run()){
		drawFrame();
	}
	destroy();
	delete cb;
}