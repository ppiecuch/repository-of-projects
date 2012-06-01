// Dialog1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "31MultiWindow.h"
#include "Dialog1.h"


// CDialog1 �Ի���



IMPLEMENT_DYNAMIC(CDialog1, CDialog)

CDialog1::CDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog1::IDD, pParent)//,pTexture(NULL)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog1, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialog1 ��Ϣ�������

void CDialog1::InitGL()
{
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_SHORT, 0, mVertices);

	glMatrixMode(GL_PROJECTION);
	glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.1f,0.2f,0.3f,1);
	glColor4x(0, 0x10000, 0, 0);
}

void CDialog1::DrawFrame()
{
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
		TRACE("Could not Bind Contexts and Drawables for OpenGL-ES1 display.");
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	pTexture->bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_SHORT, 0, mVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mTexcoords);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glRotatex(2<<16, 0, 0, 0x10000);
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	//eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

int CDialog1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_hWnd=this->GetSafeHwnd();
	if(m_hWnd==NULL)
	{
		AfxMessageBox(TEXT("��ȡ��Ⱦ�����ڶ���ʱHWNDΪ��"));
		return -1;
	}
	TRACE("1m_hWnd:%d\n",m_hWnd);
	m_hDc = ::GetDC(m_hWnd);
	m_eglDisplay = eglGetDisplay(m_hDc);
	//m_eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	//eglBindAPI(EGL_OPENGL_ES_API); 
	if(eglGetError() != EGL_SUCCESS){
		TRACE("EGL_ERROR\n");
	}
	EGLint majorVersion, minorVersion;
	if (!eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		::MessageBox(NULL,TEXT("Could not initialize OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	else
	{
		TRACE("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}
	EGLint attribs[] =
	{
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 5,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, 1,
		EGL_BUFFER_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE, 16,
		EGL_STENCIL_SIZE, false,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_SAMPLES, 0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_NONE, 0
	};
	EGLConfig config;
	EGLint num_configs;
	if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
	{
		::MessageBox(NULL,TEXT("Could not get config for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, m_hWnd, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
		//why?
		//Through platform specific functions we need to create a window that we can
		//use for OpenGL ES output.If there is no window system win should remain 0.
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
	{
		::MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);
	if(m_eglContext==EGL_NO_CONTEXT){
		::MessageBox(NULL,TEXT("Could not create EGLContext for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
		::MessageBox(NULL,TEXT("Could not Bind Contexts and Drawables for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}

	InitGL();

	pTexture=NULL;
	pTexture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\rock.png");

	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣

	return 0;
}

void CDialog1::OnDestroy()
{
	CDialog::OnDestroy();

	TRACE("1OnDestroy start\n");

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(WM_RENDER_FRAME);

	if(pTexture)
		delete pTexture;

	if(eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)==EGL_TRUE){
		printf("Unbind GL contexts and surface\n");
	}else{
		::MessageBox(NULL,TEXT("Could not Unbind GL contexts and surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
	}
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglDestroySurface(m_eglDisplay, m_eglSurface);
	if(eglTerminate(m_eglDisplay)==EGL_TRUE){
		printf("Release all resources in EGL and display\n");
	}else{
		::MessageBox(NULL,TEXT("Could not Release all resources in EGL and display for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
	}
	if (m_hDc&&!::ReleaseDC((HWND)m_hWnd, m_hDc)){
		::MessageBox(NULL,TEXT("Could not Release DC for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
	}else{
		printf("Release DC");
	}
	TRACE("1OnDestroy end\n");
}

BOOL CDialog1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CDialog1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	DrawFrame();
}

void CDialog1::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CDialog1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//���´��������ͻ�����ͬʱ�ػ�ʱ������������
		InvalidateRect(NULL,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialog1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	/*cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS |
		CS_HREDRAW |
		CS_VREDRAW |
		CS_SAVEBITS |
		CS_NOCLOSE |
		//CS_OWNDC
		CS_CLASSDC
		,AfxGetApp()->LoadStandardCursor(IDC_ARROW), 0 ,AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));*/

	return CDialog::PreCreateWindow(cs);
}
