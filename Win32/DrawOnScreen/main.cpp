#include <windows.h>
#include "wtypes.h"
#include <iostream>

const char g_szClassName[] = "testWindowClass";


// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

DWORD WINAPI MyThread(LPVOID lpParameter)
{
	HDC hdc;
	SIZE s;
	s.cx = ::GetSystemMetrics(SM_CXSCREEN);
	s.cy = ::GetSystemMetrics(SM_CYSCREEN);
	int x, y, z, r, g, b;
	int horizontal = 0;
	int vertical = 0;
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
	HBRUSH hbr, hbrOld;
	x = horizontal / 4; //s.cx * rand() / RAND_MAX;  // position x
	y = vertical / 4; //s.cy * rand() / RAND_MAX;  // position y
	z = 30; // radius
	r = 255; // red color componennt
	g = 200;// green color component
	b = 0;// blue color component
	hbr = ::CreateSolidBrush(RGB(r,g,b));
	hdc = ::GetDCEx(NULL, 0, 0);
	hbrOld = (HBRUSH) ::SelectObject(hdc, hbr);
	while (true)
	{
		::Ellipse(hdc, x - z, y - z, x + z, y + z);
		::Sleep(10);
	}
	::SelectObject(hdc, hbrOld);
	::DeleteObject(hbr);
	::ReleaseDC(NULL, hdc);
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	::CreateThread(0, 0, MyThread, 0, 0, 0);
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Cannot Register Window!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Test Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Cannot Create Window!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}