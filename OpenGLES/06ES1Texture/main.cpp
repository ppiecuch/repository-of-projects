#include "config.h"

#include <crtdbg.h>
#include <typeinfo>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

//加载Window程序需要的头文件
#include<windows.h>
#include <stdio.h>

TCHAR szWindowClass[100] = TEXT("GLES1");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;

//业务相关
#include "sphere.h"
#if 0
const unsigned char mIndices[][4]={
	{0,1,3,2},//Front
	{7,4,6,5},//Back
	{3,2,5,6},//Top
	{1,0,7,4},//Bottom
	{2,1,6,7},//Right
	{0,3,4,5} //Left
};
const float mVertices[] ={
	-1, -1,  1,//前左下0
	1, -1,  1,//前右下1
	1,  1,  1,//前右上2
	-1,  1,  1,//前左上3
	-1, -1, -1,//后左下4
	-1,  1, -1,//后左上5
	1,  1, -1,//后右上6
	1, -1, -1 //后右下7
};
const float mCoords[]= {
	0,0,
	1,0,
	1,1,
	0,1,
	1,1,
	0,1,
	1,1,
	1,0
};
#elif 0
const unsigned char mIndices[][4]={
	{0,1,3,2}
};
const float mVertices[] ={
	-1, -1,  1,//前左下0
	1, -1,  1,//前右下1
	1,  1,  1,//前右上2
	-1,  1,  1//前左上3
};
const float mCoords[]= {
	0,0,
	1,0,
	1,1,
	0,1
};
#endif
Sphere* sphere=NULL;
CTexture* pTexture=NULL;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg, end;
#define FRAME 10
unsigned int frame=0;


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

//初始化窗口
BOOL InitWindow(HWND *result,int width, int height)
{
	WNDCLASS wc;
	RECT wRect;
	HWND hwnd;
	HINSTANCE hInstance;

	wRect.left = 0L;
	wRect.right = (long)width;
	wRect.top = 0L;
	wRect.bottom = (long)height;

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

	RegisterClass(&wc);

	//该函数依据所需客户矩形大小，计算需要的窗口矩形的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。 
	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		szWindowClass, TEXT("OpenGL ES1 Framework"), 
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
		0, 0, 
		wRect.right-wRect.left, wRect.bottom-wRect.top, 
		NULL, NULL, 
		hInstance, 
		NULL
		);

	if (!hwnd)
	{
		MessageBox(NULL,TEXT("Error in CreateWindow()!"),TEXT("Error"),MB_OK);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	*result = hwnd;

	return true;
}

BOOL InitEGL(HWND& hwnd)
{
	EGLint attribs[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 5,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, 1,
		EGL_BUFFER_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE, 1,
		EGL_STENCIL_SIZE, false,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_SAMPLES, 0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_NONE, 0
	};
	EGLConfig config;
	EGLint num_configs;
	HDC hdc = GetDC(hwnd);
	m_eglDisplay = eglGetDisplay(hdc);
	EGLint majorVersion, minorVersion;
	if ( !eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		MessageBox(NULL,TEXT("Could not initialize OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return false;
	}
	else
	{
		printf("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}

	if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
	{
		MessageBox(NULL,TEXT("Could not get config for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, hwnd, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
	{
		MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return false;
	}

	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);

	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

	//打印版本信息
	GLubyte* version =  (GLubyte*)glGetString(GL_VERSION);
	printf("gl version:%s\n", version);
	const char* eglClient=eglQueryString(m_eglDisplay, EGL_CLIENT_APIS);
	printf("egl client api:%s\n", eglClient);
	GLubyte* vendorName = (GLubyte*)glGetString( GL_VENDOR );
	printf("verdor name:%s\n", vendorName);

	return true;

}

void DestroyEGL()
{
	eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglDestroySurface(m_eglDisplay, m_eglSurface);
	eglTerminate(m_eglDisplay);
}

void InitGL(int width,int height)
{	
	/*创建球体*/
	sphere=new Sphere(8.0f,24,12);
	sphere->toString();

	pTexture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\06ES1Texture\\error.png");

	glEnable(GL_DEPTH_TEST);

#if 0
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mVertices);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, mCoords);
#else
	glEnable(GL_TEXTURE_2D);
	pTexture->bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,sphere->getVertexs());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, sphere->getTexCoords() );
#endif
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);
	gluPerspective(60.0f,(float)width/(float)height,0.1f,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,10,30,0,10,-50,0,1,0);
	glClearColor(0.1f,0.2f,0.3f,1);
	glColor4f(1, 1, 1, 1);
}

void DrawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#if 0
	for(int i=0;i<6;++i)
	{
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, mIndices[i]);
	}
	glRotatef(2, 1, 0, 1);
#else
	for(int i=0;i<sphere->getPortraitSteps();i++)
		for(int j=0;j<sphere->getHorizonalSteps();j++)
			glDrawArrays(GL_TRIANGLE_FAN,(i*sphere->getHorizonalSteps()+j)<<2,4);
	glRotatef(2, 0, 1, 0);
#endif
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void DestroyGL()
{
	if(pTexture)
		delete pTexture;
	if(sphere)
		delete sphere;
}

int main(void) {

	EnableMemLeakCheck();

	HWND hwnd;
	MSG sMessage;
	bool bDone = false;
	wchar_t buffer[128];

	int width=640;
	int height=480;

	InitWindow(&hwnd,width,height);

	InitEGL(hwnd);
	InitGL(width,height);

	while(!bDone)
	{
		if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE))
		{
			if(sMessage.message == WM_QUIT)
			{
				bDone = true;
			}
			else 
			{
				TranslateMessage(&sMessage);
				DispatchMessage(&sMessage);
			}
		}

		if(frame==0)
			_ftime64_s(&beg); 
		DrawFrame();
		if(frame==FRAME)
		{
			_ftime64_s(&end); 
			total_time = (float)(end.time - beg.time) + (float)(end.millitm - beg.millitm) / 1000;
			swprintf_s(buffer,L"OpenGL ES1 Framework(fps: %.2f)", (float)(FRAME+1)/total_time);
			SetWindowText(hwnd,buffer);
			frame=0;
		}
		else
		{
			++frame;
		}

		Sleep(20);
	}

	DestroyGL();
	DestroyEGL();
	DestroyWindow(hwnd);

	return 0;
}
