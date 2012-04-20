#include "ThreadWin.h"
#pragma comment( lib,"opengl32.lib")
#pragma comment( lib,"glu32.lib")
#include <GL/gl.h>      // OpenGL definitions
#include <stdio.h>
#include <assert.h>

#include "Monitor.h"

static WNDCLASSEX wcx;
static char szAppName[] = "OpenGL";
HINSTANCE _hInstance;


MouseState mouse = {
	0,
	0,
	false,
	false,
	false,
};

#define TESTFLAGS (PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL)

//******************************************************************
//******************************************************************

DWORD tlsIndex;     // Index for thread local data.


//******************************************************************
//******************************************************************


//========================================================================
// Setup a pixel format for a window, create context and initialize 
// OpenGL 1.2 procs.

BOOL SetupWindow (HWND hWnd, WindowData *pwdata)
{
	int iPixelFormat = 0;

	static	PIXELFORMATDESCRIPTOR pfd=		// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
		1,					// Version Number
		PFD_DRAW_TO_WINDOW |			// Format Must Support Window
		PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,			// Must Support Double Buffering
		PFD_TYPE_RGBA,				// Request An RGBA Format
		32,					// Select Our Color Depth
		0, 0, 0, 0, 0, 0,			// Color Bits Ignored
		0,					// No Alpha Buffer
		0,					// Shift Bit Ignored
		0,					// No Accumulation Buffer
		0, 0, 0, 0,				// Accumulation Bits Ignored
		16,					// 16Bit Z-Buffer (Depth Buffer)  
		0,					// No Stencil Buffer
		0,					// No Auxiliary Buffer
		PFD_MAIN_PLANE,				// Main Drawing Layer
		0,					// Reserved
		0, 0, 0					// Layer Masks Ignored
	};

	if (!(pwdata->hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(iPixelFormat=ChoosePixelFormat(pwdata->hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(pwdata->hDC,iPixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(pwdata->hRC=wglCreateContext(pwdata->hDC)))				// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(pwdata->hDC,pwdata->hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	// Initialize the OpenGL context.

	pwdata->InitOpenGL(pwdata);

	return TRUE;
}

//========================================================================
// Proc to receive window messages
LONG WINAPI OnKeyDown(unsigned int key, WindowData* pwdata)
{
	RECT rc;

	switch (key)
	{
	case 27:
		PostMessage(pwdata->hWnd, WM_DESTROY, 0, 0);

		break;

	case VK_F11:
		if (pwdata->bFullscreen)
		{
			MoveWindow(pwdata->hWnd,
				pwdata->xpos,
				pwdata->ypos,
				800,
				600,
				TRUE);
		} 
		else
		{
			GetWindowRect(pwdata->hWnd, &rc);
			pwdata->xpos = rc.left;
			pwdata->ypos = rc.top;
	 		pwdata->width  = rc.right - rc.left;
	 		pwdata->height = rc.bottom - rc.top;


			GetMonitorRect(&rc, GetMonitor(pwdata->display));

			MoveWindow(pwdata->hWnd,
				rc.left, rc.top,
				rc.right - rc.left, rc.bottom - rc.top,
				TRUE);

		}

		pwdata->bFullscreen = !pwdata->bFullscreen;

		break;
	}

	pwdata->KeyDown(key, pwdata);

	return 0;
}

LONG WINAPI OnPaint(HWND wParam, WindowData* lParam)
{
	PAINTSTRUCT ps;
	HWND hWnd = wParam;
	WindowData* pwdata = lParam;

	//BeginPaint (hWnd, &ps);

	pwdata->DrawScene(pwdata);          // Update the client area
	SwapBuffers (pwdata->hDC);

	//EndPaint (hWnd, &ps);

	return 0;
}

LONG WINAPI WndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowData  *pwdata;
	POINTS p;
	RECT rect;
	// Obtain the window data for this thread. Note that WM_CREATE cannot
	// "read" the values in window data since it has not yet been setup until
	// we have gone through creation/initialization.

	pwdata = (WindowData *)TlsGetValue (tlsIndex);

	// Process the window message.
	switch (msg)
	{
	case WM_CREATE:
		if (!SetupWindow (hWnd, (WindowData *)((LPCREATESTRUCT) lParam)->lpCreateParams))							// Did We Get A Device Context?
		{
			MessageBox(NULL,"Can't Setup the Window For OpenGL Rendering.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;			// Return FALSE
		}

		return (0);

	case WM_SIZE:
 		pwdata->width  = LOWORD (lParam);
 		pwdata->height = HIWORD (lParam);

		glViewport (0, 0, pwdata->width, pwdata->height);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

		//glOrtho(0, pwdata->width, 0, pwdata->height, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		return 0;

	case WM_MOVE:
		break;

	case WM_LBUTTONDOWN:
		p = MAKEPOINTS(lParam);
		mouse.x = p.x;
		mouse.y = p.y;
		mouse.lbdown = true;
		
		break;

	case WM_LBUTTONUP:
		//p = MAKEPOINTS(lParam);
		//mouse.x = p.x;
		//mouse.y = p.y;
		mouse.lbdown = false;

		break;

	case WM_MOUSEMOVE:
		p = MAKEPOINTS(lParam);
		GetWindowRect(pwdata->hWnd, &rect);
		/*MK_CONTROL?*/
		if (mouse.lbdown)
		{
			SetWindowPos(
				pwdata->hWnd,
				0,
				rect.left-mouse.x + p.x,
				rect.top-mouse.y + p.y,
				pwdata->width,
				pwdata->height,
				SWP_NOSIZE);
		}
		break;


	case WM_ERASEBKGND:

		break;

	case WM_PAINT:
		// Since each thread has its own context, we don't have
		// to do a MakeCurrent for each paint message.
		OnPaint(hWnd, pwdata);

		return 0;       

	case WM_KEYDOWN:
		OnKeyDown((unsigned int)wParam, pwdata);
		return 0;       

	case WM_DESTROY:
		// Delete the context. 

		wglDeleteContext (pwdata->hRC);

		PostQuitMessage( 0 );
		if (pwdata->bMainwindow)
		{
			ExitProcess(0);
		}
		return 0;
	}

	return DefWindowProc (hWnd, msg, wParam, lParam);
}

//========================================================================

int ThreadMain (void *vi)
{
	int i = (int) vi;

	MSG msg;  // message struct 

	RECT rect;

	//******************************************************************
	//******************************************************************
	// Store the windowData for this thread in the thread local data.

	TlsSetValue (tlsIndex, &wdata[i]);

	//******************************************************************
	//******************************************************************

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcx.cbSize = sizeof(wcx);           // size of structure 
	wcx.style = CS_HREDRAW | 
		CS_VREDRAW | CS_OWNDC;                 // redraw if size changes 
	wcx.lpfnWndProc = WndProc;          // points to window procedure 
	wcx.cbClsExtra = 0;                 // no extra class memory 
	wcx.cbWndExtra = 0;                 // no extra window memory 
	wcx.hInstance = _hInstance;          // handle to instance 
	wcx.hIcon = LoadIcon(NULL, 
		IDI_APPLICATION);               // predefined app. icon 
	wcx.hCursor = LoadCursor(NULL, 
		IDC_ARROW);                     // predefined arrow 
	wcx.hbrBackground = (HBRUSH)GetStockObject( 
		WHITE_BRUSH);                   // white background brush 
	wcx.lpszMenuName =  NULL;           // name of menu resource 
	wcx.lpszClassName = szAppName;      // name of window class 
	wcx.hIconSm = (HICON)LoadImage(_hInstance, // small class icon 
		MAKEINTRESOURCE(5),
		IMAGE_ICON, 
		GetSystemMetrics(SM_CXSMICON), 
		GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR); 

	// Register the window class. 

	RegisterClassEx(&wcx);

	if (wdata[i].bFullscreen)
	{
		GetMonitorRect(&rect, GetMonitor(wdata[i].display));
	}
	else
	{
		rect.left = wdata[i].xpos;
		rect.top = wdata[i].ypos;
		rect.right = wdata[i].width + rect.left;
		rect.bottom = wdata[i].height + rect.top;

	}
	// Create a window for the application.

	wdata[i].hWnd = CreateWindow(
		szAppName,              // Application name
		wdata[i].windowTitle,   // Window title text
		wdata[i].windowStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,

		NULL,                   // No parent window
		NULL,                   // Use the window class menu.
		_hInstance,             // This instance owns this window
		&wdata[i]);             // Application window data structure

	// Check for window creation

	if (!wdata[i].hWnd) {
		assert (0);
		exit (1);
	}

	// Make the window visible & update its client area

	ShowWindow (wdata[i].hWnd, SW_SHOWNORMAL);
	UpdateWindow (wdata[i].hWnd);            // Send WM_PAINT message

	// Enter the Windows message loop. Get and dispatch messages
	// until WM_QUIT.

	while (GetMessage(&msg, // message structure
		NULL,        // handle of window receiving the message
		0,           // lowest message id to examine
		0)) {        // highest message id to examine
			TranslateMessage (&msg);
			DispatchMessage (&msg);
	}

	UnregisterClass(szAppName, _hInstance);

	return (msg.wParam);
}

//========================================================================
// Windows main program.

int WINAPI WinMain (HINSTANCE hInstance,
		    HINSTANCE hPrevInstance,
		    LPSTR lpszCmdLine, int nCmdShow)
{

	WNDCLASS wc;   // windows class sruct

	int      i;
	HANDLE   hThread;   
	DWORD    IDThread;

	// 初始化显示器信息
	InitMonitorInfo();

	_hInstance = hInstance; // Save for later use.

	// Fill in window class structure with parameters that
	//  describe the main window.

	wc.style         = CS_HREDRAW | CS_VREDRAW;     // Class style(s).
	wc.lpfnWndProc   = (WNDPROC)WndProc;            // Window Procedure
	wc.cbClsExtra    = 0;     
	wc.cbWndExtra    = 0;    
	wc.hInstance     = hInstance;                   // Owner of this class
	wc.hIcon         = NULL;                        // Icon name 
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);    // Default color
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = szAppName;                   // Name to register as

	// Register the window class

	RegisterClass( &wc );

	//******************************************************************
	//******************************************************************
	// For multi-threaded applications, allocate a thread local storage
	// for storing a global for each thread.

	if ( (tlsIndex = TlsAlloc()) == -1) {
		assert (0);
		exit(1);
	}

	//******************************************************************
	//******************************************************************
	for (i = 0; i < winCount; i++) {
		hThread = CreateThread (NULL, 0, 
			(LPTHREAD_START_ROUTINE) ThreadMain, 
			(void *) i, 0, &IDThread);

		if (hThread == NULL) {
			assert (0);
			exit(1);
		}
	}

	// We don't nee this thread anymore since the two threads are 
	// taking care of the two windows independently. However, we 
	// exit the thread and not the process.

	ExitThread (0);
}
