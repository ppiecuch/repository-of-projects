#include <stdio.h>

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <windows.h>
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

struct 
{
	int iStyle;
	TCHAR* szText;
}
button=
{
	BS_PUSHBUTTON,	TEXT("PUSHBUTTON")
};
HWND hWnd;
int ID_BUTTON=0;

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	/*MSG msg;
	HWND hWnd;
	wchar_t szTitle[]=L"ϵͳ��Ϣ";                                // The title bar text
	wchar_t szWindowClass[]=L"popMsg";                                // The title bar text
	WNDCLASSEX wcex={0};
	wcex.cbSize = sizeof(WNDCLASSEX);        //WNDCLASSEX�ṹ���С
	wcex.style            = CS_HREDRAW | CS_VREDRAW;    //λ�øı�ʱ�ػ�
	wcex.lpfnWndProc    = (WNDPROC)WndProc;            //��Ϣ������
	wcex.hInstance        = 0;            //��ǰʵ�����
	wcex.hbrBackground    = (HBRUSH)COLOR_WINDOWFRAME;    //����ɫ
	wcex.lpszClassName    = szWindowClass;        //�δ�������
	wcex.hIcon            =0;        //ͼ��
	wcex.hCursor        =LoadCursor(NULL, IDC_ARROW);        //���
	wcex.lpszMenuName    =0;        //�˵�����
	wcex.hIconSm        =0;        //��С��ͼ��
	RegisterClassEx(&wcex);            //ע�ᴰ����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU|WS_VISIBLE,    //��������
		CW_USEDEFAULT,CW_USEDEFAULT, 400, 300, NULL, NULL, 0, NULL);
	if (!hWnd){
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))     // ��Ϣѭ��:
	{
		TranslateMessage(&msg);        //ת�����ⰴ�����ַ���Ϣ
		DispatchMessage(&msg);        //������Ϣ���ûص�����
	}
	return msg.wParam;

	return 0;*/

	MSG msg          = {0};
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"minwindowsapp";
	if( FAILED(RegisterClass(&wc)) )
		return 1;

	
	hWnd=CreateWindow(wc.lpszClassName,
		L"Minimal Windows Application",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		0,0,640,480,0,0,0,NULL);
	if(!hWnd)
		return 2;

	while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
	{
		DispatchMessage( &msg );
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar,cyChar;
	switch(message)
	{
	case WM_CREATE:
		cxChar=LOWORD(GetDialogBaseUnits());
		cyChar=HIWORD(GetDialogBaseUnits());
		CreateWindow(TEXT("button"),button.szText,WS_CHILD|WS_VISIBLE|button.iStyle,cxChar,0,20*cxChar,7*cyChar/4,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,NULL);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		{
			int wNotifyCode = HIWORD(wParam);
			if(wNotifyCode==BN_CLICKED)
			{
				printf("click\n");
				return 0;
			}
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}  

/*LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	switch (message) 
	{    
	case WM_PAINT:        //�ػ���Ϣ
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		MoveToEx(hdc,10,10,0);
		SelectObject( hdc,::CreatePen(PS_SOLID,1,RGB(0xFF,0,0)));
		LineTo(hdc,100,200);
		//            DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		RegisterHotKey(hWnd ,1, 0 ,0x41 );
		break;
	case WM_HOTKEY:
		MessageBox(0,0,0,0);
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY:    //����������Ϣ
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}*/