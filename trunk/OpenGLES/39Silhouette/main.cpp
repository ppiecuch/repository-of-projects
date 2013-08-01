//����OpenGL ES1��Ҫ�Ŀ⼰ͷ�ļ�
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>

//����Window������Ҫ��ͷ�ļ�
#include<windows.h>
#include <stdio.h>

TCHAR szWindowClass[100] = TEXT("GLES1");

EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;

//ҵ�����
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
#include "sys/timeb.h"//ͷ�ļ��а���
float total_time;
struct _timeb beg, end;
#define FRAME 10
unsigned int frame=0;


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

//��ʼ������
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

	RegisterClass(&wc);

	//�ú�����������ͻ����δ�С��������Ҫ�Ĵ��ھ��εĴ�С��������Ĵ��ھ��������Դ��͸�CreateWindowEx���������ڴ���һ���ͻ��������С�Ĵ��ڡ� 
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
	//Ϊ��ʹ��OpenGL��ģ�幦�ܣ����ȱ���ʹ��ƽ̨�ض���OpenGL���ù�������һ��ģ�建������
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

	//��ӡ�汾��Ϣ
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

	//�ڻ�ͼ֮ǰ��Ҫȷ��ģ�建��������������ú���glClear()���������
	//�������������ʹ�õ�ֵ����ǰ��ĺ���glClearStencil(Glint s)���õġ�
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//ͨ��glEnable/glDisable�������û����ģ����ԡ�
	// ����ģ�����
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glVertexPointer(3, GL_SHORT, 0, mVertices);

	//OpenGL��ģ�建������Ϊÿ�����ر�����һ����ģ��ֵ������������Ҫ����ģ�����ʱ�����趨��ģ��ο�ֵ������صġ�ģ��ֵ�����бȽϣ�
	//����������ͨ�����ԣ��������������򱻶����������л��ơ�
	//��������ģ�����֮�󣬾�ʹ��glStenceFunc�����Ĳ�����洢��ģ�建�����ڵ�ֵ���в��ԡ�
	//���ݲ��Խ����ֵƬ�λ��߱�д�룬���߶�����
	//������
	//func
	//��ȷ˵�������Ĳ��Թ��ܡ�����8�����ų������ã��ֱ��ǣ�GL_NEVER,��GL_LESS�� GL_LEQUAL�� GL_GREATER�� GL_GEQUAL,��GL_EQUAL�� GL_NOTEQUAL�� GL_ALWAYS��
	//��ʼ����ֵ��GL_ALWAYS��
	//ref
	//��ȷ˵����ģ����Ե�����ֵ�� ref ֵ��������0~2^(n-1)�䣬����n��ģ�建����λƽ��������ʼ��ֵ��0��   
	//mask
	//�ò�����ʾһ��ģ�壬������refֵ�Լ��洢��ģ��ֵ�������㡣��ʼ��ֵ��ȫ1��
	//�磺
	//glStencilFunc(GL_LESS, 3, mask);
	//��δ�������ģ����Ե�����Ϊ����С��3��ͨ������
	//glStencilFunc��ǰ��������������glAlphaFunc�������������ƣ�����������������Ϊ��������бȽϣ���ֻ�Ƚ�mask�ж�����Ϊ1��λ
	glStencilFunc(GL_ALWAYS,1,1);
	//ģ�建���������ڲ��Թ�����Ҳ������޸ģ�����ģ�建������ֵȡ������ε���glStencilOp������
	//void glStenclOp(Glenum fail��Glenum zfail��Glenum zpass)�������еĲ���ֵ����OpenGL���ģ����Գɹ�/ʧ�ܣ�Ӧ������޸�ģ�建������ֵ��
	//ÿ�����صġ�ģ��ֵ�������ģ����ԵĽ������Ȳ��ԵĽ�������иı䡣
	//�ú���ָ������������¡�ģ��ֵ������α仯����һ��������ʾģ�����δͨ��ʱ����α仯���ڶ���������ʾģ�����ͨ��������Ȳ���δͨ��ʱ����α仯��������������ʾģ����Ժ���Ȳ��Ծ�ͨ��ʱ����α仯�����û������ģ����ԣ�����Ϊģ���������ͨ�������û��������Ȳ��ԣ�����Ϊ��Ȳ�������ͨ����
	//�仯�����ǣ�
	//GL_KEEP�����ı䣬��Ҳ��Ĭ��ֵ����
	//GL_ZERO�����㣩��
	//GL_REPLACE��ʹ�ò��������е��趨ֵ�����浱ǰģ��ֵ����
	//GL_INCR������1��������Ѿ������ֵ���򱣳ֲ��䣩��
	//GL_INCR_WRAP������1��������Ѿ������ֵ����������¿�ʼ����
	//GL_DECR������1��������Ѿ����㣬�򱣳ֲ��䣩��
	//GL_DECR_WRAP������1��������Ѿ����㣬����������Ϊ���ֵ����
	//GL_INVERT����λȡ������
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

	//ȥ��ʵ�岿�֣�ֻ��������
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
