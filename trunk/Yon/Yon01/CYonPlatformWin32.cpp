#include "CYonPlatformWin32.h"

yon::platform::CYonPlatformWin32::CYonPlatformWin32(const yon::SYonEngineParameters& params)
	:m_hWnd(NULL)
{
	initWindow(params);
}
yon::platform::CYonPlatformWin32::~CYonPlatformWin32(){
	DestroyWindow(m_hWnd);
}

//消息处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	switch(uiMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SIZE:
		return 0;
	}
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}
bool yon::platform::CYonPlatformWin32::initWindow(const yon::SYonEngineParameters& params){

	WNDCLASS wc;
	RECT wRect;
	HINSTANCE hInstance;
	const c16* szWindowClass=TEXT("CYonPlatformWin32");

	wRect.left = 0L;
	wRect.right = (long)params.windowSize.w;
	wRect.top = 0L;
	wRect.bottom = (long)params.windowSize.h;

	//用GetModuleHandle()函数获得当前程序实例句柄
	hInstance = GetModuleHandle(NULL);

	//通常，在您呼叫GetDC或BeginPaint时，Windows用默认值建立一个新的设备内容，
	//您对属性所做的一切改变在设备内容用 ReleaseDC或EndPaint呼叫释放时，都会丢失。
	//虽然在通常情况下这种方法已经很令人满意了，但是您还可能想要在释放设备内容之后，
	//仍然保存程序中对设备内容属性所做的改变，以便在下一次呼叫GetDC 和BeginPaint时
	//它们仍然能够起作用。为此，可在登录窗口类别时，将CS_OWNDC旗标纳入窗口类别的一部分：
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;

	//注册窗口类
	RegisterClass(&wc);

	//该函数依据所需客户矩形大小，计算需要的窗口矩形的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。 
	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		szWindowClass, params.windowCaption.c_str(), 
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		0, 0, 
		wRect.right-wRect.left, wRect.bottom-wRect.top, 
		NULL, NULL, 
		hInstance, 
		NULL
		);

	YON_DEBUG_BREAK_IF(!m_hWnd)

	if (!m_hWnd)
	{
		MessageBox(NULL,TEXT("Error in CreateWindow()!"),TEXT("Error"),MB_OK);
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

//yon::ITimer* yon::platform::CYonPlatformWin32::getTimer(){return NULL;}