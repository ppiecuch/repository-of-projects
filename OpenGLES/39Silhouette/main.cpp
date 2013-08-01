//加载OpenGL ES1需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>

//加载Window程序需要的头文件
#include<windows.h>
#include <stdio.h>

TCHAR szWindowClass[100] = TEXT("GLES1");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;

//业务相关
unsigned char mIndices[] = { 0, 1, 2 };
signed short mVertices[] = {
	-50, -29, 0,
	50, -29, 0,
	0,  58, 0
};
signed short mVertices2[] = {
	-100, -58, 0,
	100, -58, 0,
	0,  116, 0
};
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
	//为了使用OpenGL的模板功能，首先必须使用平台特定的OpenGL设置过程请求一个模板缓存区。
	EGLint attribs[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 5,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, 1,
		EGL_BUFFER_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE, 1,
		EGL_STENCIL_SIZE, 1,
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

void InitGL()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_SHORT, 0, mVertices);

	glMatrixMode(GL_PROJECTION);
	glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.1f,0.2f,0.3f,1);
	
}

void DrawFrame(int width,int height)
{
	/*glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);

	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);*/

	//在绘图之前，要确保模板缓存区被清除，调用函数glClear()来清除缓存
	//在清除操作中所使用的值是由前面的函数glClearStencil(Glint s)设置的。
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//通过glEnable/glDisable可以启用或禁用模板测试。
	// 启用模板测试
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glVertexPointer(3, GL_SHORT, 0, mVertices);

	//OpenGL在模板缓冲区中为每个像素保存了一个“模板值”，当像素需要进行模板测试时，将设定的模板参考值与该像素的“模板值”进行比较，
	//符合条件的通过测试，不符合条件的则被丢弃，不进行绘制。
	//在启用了模板测试之后，就使用glStenceFunc函数的参数与存储在模板缓存区内的值进行测试。
	//根据测试结果，值片段或者被写入，或者丢弃。
	//参数：
	//func
	//明确说明函数的测试功能。共有8个符号常量可用，分别是：GL_NEVER,，GL_LESS， GL_LEQUAL， GL_GREATER， GL_GEQUAL,，GL_EQUAL， GL_NOTEQUAL和 GL_ALWAYS。
	//初始化的值是GL_ALWAYS。
	//ref
	//明确说明该模板测试的引用值。 ref 值被限制在0~2^(n-1)间，其中n是模板缓存中位平面数。初始化值是0。   
	//mask
	//该参数表示一个模板，用来和ref值以及存储的模板值做与运算。初始化值是全1。
	//如：
	//glStencilFunc(GL_LESS, 3, mask);
	//这段代码设置模板测试的条件为：“小于3则通过”。
	//glStencilFunc的前两个参数意义与glAlphaFunc的两个参数类似，第三个参数的意义为：如果进行比较，则只比较mask中二进制为1的位
	glStencilFunc(GL_ALWAYS,1,1);
	//模板缓存区本身在测试过程中也会进行修改，进入模板缓存区的值取决于如何调用glStencilOp函数：
	//void glStenclOp(Glenum fail，Glenum zfail，Glenum zpass)。函数中的参数值告诉OpenGL如果模板测试成功/失败，应该如何修改模板缓存区的值。
	//每个像素的“模板值”会根据模板测试的结果和深度测试的结果而进行改变。
	//该函数指定了三种情况下“模板值”该如何变化。第一个参数表示模板测试未通过时该如何变化；第二个参数表示模板测试通过，但深度测试未通过时该如何变化；第三个参数表示模板测试和深度测试均通过时该如何变化。如果没有起用模板测试，则认为模板测试总是通过；如果没有启用深度测试，则认为深度测试总是通过）
	//变化可以是：
	//GL_KEEP（不改变，这也是默认值），
	//GL_ZERO（回零），
	//GL_REPLACE（使用测试条件中的设定值来代替当前模板值），
	//GL_INCR（增加1，但如果已经是最大值，则保持不变），
	//GL_INCR_WRAP（增加1，但如果已经是最大值，则从零重新开始），
	//GL_DECR（减少1，但如果已经是零，则保持不变），
	//GL_DECR_WRAP（减少1，但如果已经是零，则重新设置为最大值），
	//GL_INVERT（按位取反）。
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	for(int i=-1;i<=1;i+=2)
	{
		for(int j=-1;j<=1;j+=2)
		{
			glViewport(i,j,width+i,height+j);
			//glPushMatrix();
			//glTranslatef(i,j,0);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);
			//glPopMatrix();
		}
	}

	glViewport(0,0,width,height);

	//去掉实体部分，只保留轮廓
	glStencilFunc(GL_ALWAYS, 0, 0);
	glColor4x(0x10000, 0, 0, 0);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glColor4x(0, 0x10000, 0, 0);
	glVertexPointer(3, GL_SHORT, 0, mVertices2);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);

	glDisable(GL_STENCIL_TEST);
	glColor4x(0x10000, 0, 0, 0);
	glVertexPointer(3, GL_SHORT, 0, mVertices);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);

	glRotatex(2<<16, 0, 0, 0x10000);
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

int main(void) {

	HWND hwnd;
	MSG sMessage;
	bool bDone = false;
	wchar_t buffer[128];

	int width=640;
	int height=480;

	InitWindow(&hwnd,width,height);

	InitEGL(hwnd);
	InitGL();
	
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
		DrawFrame(width,height);
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

	DestroyEGL();
	DestroyWindow(hwnd);

	return 0;
}
