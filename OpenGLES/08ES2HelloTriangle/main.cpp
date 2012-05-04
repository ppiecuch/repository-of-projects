
/************************************************************************/
/*  This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering.
/************************************************************************/
//加载OpenGL ES2需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#include <EGL/egl.h>
#include <GLES2/gl2.h>

//加载Window程序需要的头文件
#include<windows.h>
#include <stdio.h>

#define GL_CHECK \
		{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
		printf("glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
		}else{ \
		printf("gl process ok at line %i\n",  __LINE__); \
		} \
		}

TCHAR szWindowClass[100] = TEXT("GLES2");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;

//业务相关
//顶点属性索引
#define VERTEX_ARRAY 0
const float aVertices[] ={  
	 0.0f,  0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f 
};

GLint iLocPosition = 0;
const unsigned int uiWidth  = 640;
const unsigned int uiHeight = 480;

#include "sys/timeb.h"//头文件中包含
float total_time;
struct _timeb beg,end;
#define FRAME 10
unsigned int frame=0;

//shader
const char* pszVertShader= 
				"attribute vec4 vPosition;    \n"
				"void main()                  \n"
				"{                            \n"
				"   gl_Position = vPosition;  \n"
				"}                            \n";
const char* pszFragShader=
				"precision mediump float;\n"\
				"void main()                                  \n"
				"{                                            \n"
				"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
				"}                                            \n";
GLuint uiVertShader;
GLuint uiFragShader;
GLuint uiProgramObject;



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

	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		szWindowClass, TEXT("OpenGL ES2 HelloTriangle"), 
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
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE, 0
	};
	EGLConfig config;
	EGLint num_configs;
	HDC hdc = GetDC(hwnd);
	m_eglDisplay = eglGetDisplay(hdc);
	EGLint majorVersion, minorVersion;
	if ( !eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		MessageBox(NULL,TEXT("Could not initialize OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}
	else
	{
		printf("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}

	if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
	{
		MessageBox(NULL,TEXT("Could not get config for OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, hwnd, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
	{
		MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES2 display."),TEXT("Error"),MB_OK);
		return false;
	}

	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);

	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);


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
	uiVertShader = glCreateShader(GL_VERTEX_SHADER);
	uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(uiVertShader,1,(const char**)&pszVertShader,NULL);
	glShaderSource(uiFragShader,1,(const char**)&pszFragShader,NULL);

	glCompileShader(uiVertShader);
	glCompileShader(uiFragShader);

	GLint iShaderCompiled;
	glGetShaderiv(uiVertShader,GL_COMPILE_STATUS,&iShaderCompiled);
	if(!iShaderCompiled)
	{
		int i32LogLength,i32CharsWritten;
		glGetShaderiv(uiVertShader,GL_INFO_LOG_LENGTH,&i32LogLength);

		char* pszLog = new char[i32LogLength];
		glGetShaderInfoLog(uiVertShader,i32LogLength,&i32CharsWritten,pszLog);

		MessageBoxA(NULL,"Failed to compile vertex shader:%s",pszLog,MB_OK);

		delete[] pszLog;
		return;
	}

	glGetShaderiv(uiFragShader,GL_COMPILE_STATUS,&iShaderCompiled);
	if(!iShaderCompiled)
	{
		int i32LogLength,i32CharsWritten;
		glGetShaderiv(uiFragShader,GL_INFO_LOG_LENGTH,&i32LogLength);
		char* pszLog = new char[i32LogLength];
		glGetShaderInfoLog(uiFragShader,i32LogLength,&i32CharsWritten,pszLog);
		MessageBoxA(NULL,"Failed to compile fragment shader:%s",pszLog,MB_OK);
		delete[] pszLog;
		return;
	}

	uiProgramObject = glCreateProgram();

	glAttachShader(uiProgramObject,uiFragShader);
	glAttachShader(uiProgramObject,uiVertShader);

	//将通用顶点数据传递给OpenGL有两种方式：
	//1、让链接程序自动分配绑定。在这种情况下，应用程序需要在链接之后查询OpenGL，以便确定所分配的通用顶点索引，
	//然后在将属性传递给OpenGL时使用这些索引；
	//2、第二种方式是让应用程序在链接之前使用glBindAttribLocation函数来选择要使用的通用顶点属性索引值，将将它
	//明确地绑定到顶点着色器中的特定属性变量上。
	// Bind vPosition to attribute VERTEX_ARRAY
	glBindAttribLocation (uiProgramObject, VERTEX_ARRAY, "vPosition" );

	glLinkProgram(uiProgramObject);

	GLint iLinked;
	glGetProgramiv(uiProgramObject,GL_LINK_STATUS,&iLinked);
	if(!iLinked)
	{
		int ui32LogLength,ui32CharsWritten;
		glGetProgramiv(uiProgramObject,GL_INFO_LOG_LENGTH,&ui32LogLength);
		char* pszLog = new char[ui32LogLength];
		glGetProgramInfoLog(uiProgramObject,ui32LogLength,&ui32CharsWritten,pszLog);
		MessageBoxA(NULL,"Failed to link program:%s",pszLog,MB_OK);
		delete[] pszLog;
		return;
	}


	glUseProgram(uiProgramObject);

	//指定在渲染时使用的通用顶点属性值数组的位置和数据格式。
	//通用顶点属性都是通用的，它们可以用来传递额外的颜色值、正切、副法向量、深度值或其他任何信息。
	glVertexAttribPointer (VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, 0, aVertices );
	//指定了一个通用顶点属性数组的顶点数组信息之后，就需要启用数组。
	//启用数组时，在调用顶点数组绘制命令（如glDrawArray）时，指定数组中的通用顶点数据就会
	//随其他启用的顶点数组数据一起提供。
	//默认情况下，所有客户端功能都是禁用的。
	glEnableVertexAttribArray (VERTEX_ARRAY);
	GL_CHECK

	glViewport ( 0, 0, uiWidth, uiHeight );

	glClearColor(0.1f,0.2f,0.3f,1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void DrawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	if (!eglSwapBuffers(m_eglDisplay, m_eglSurface)) {
		printf("Failed to swap buffers.\n");
	}
}

void DestroyGL()
{
	glUseProgram(NULL);
	glDeleteProgram(uiProgramObject);
	glDeleteShader(uiVertShader);
	glDeleteShader(uiFragShader);
}

int main(void) {

	HWND hwnd;
	MSG sMessage;
	bool bDone = false;
	char buffer[128];

	InitWindow(&hwnd,uiWidth,uiHeight);

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
		DrawFrame();
		if(frame==FRAME)
		{
			_ftime64_s(&end); 
			total_time = (float)(end.time - beg.time) + (float)(end.millitm - beg.millitm) / 1000;
			sprintf_s(buffer,"OpenGL ES2 HelloTriangle(fps: %.2f)", (float)(FRAME+1)/total_time);
			SetWindowTextA(hwnd,buffer);
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