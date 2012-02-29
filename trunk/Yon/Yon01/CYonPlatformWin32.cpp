#include "CYonPlatformWin32.h"

yon::platform::CYonPlatformWin32::CYonPlatformWin32(const yon::SYonEngineParameters& params)
	:m_hWnd(NULL)
{
	initWindow(params);
}
yon::platform::CYonPlatformWin32::~CYonPlatformWin32(){
	DestroyWindow(m_hWnd);
}

//��Ϣ������
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

	//��GetModuleHandle()������õ�ǰ����ʵ�����
	hInstance = GetModuleHandle(NULL);

	//ͨ������������GetDC��BeginPaintʱ��Windows��Ĭ��ֵ����һ���µ��豸���ݣ�
	//��������������һ�иı����豸������ ReleaseDC��EndPaint�����ͷ�ʱ�����ᶪʧ��
	//��Ȼ��ͨ����������ַ����Ѿ������������ˣ���������������Ҫ���ͷ��豸����֮��
	//��Ȼ��������ж��豸�������������ĸı䣬�Ա�����һ�κ���GetDC ��BeginPaintʱ
	//������Ȼ�ܹ������á�Ϊ�ˣ����ڵ�¼�������ʱ����CS_OWNDC������봰������һ���֣�
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

	//ע�ᴰ����
	RegisterClass(&wc);

	//�ú�����������ͻ����δ�С��������Ҫ�Ĵ��ھ��εĴ�С��������Ĵ��ھ��������Դ��͸�CreateWindowEx���������ڴ���һ���ͻ��������С�Ĵ��ڡ� 
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