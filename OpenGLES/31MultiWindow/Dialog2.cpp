// Dialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "31MultiWindow.h"
#include "Dialog2.h"


// CDialog2 对话框

IMPLEMENT_DYNAMIC(CDialog2, CDialog)

CDialog2::CDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2::IDD, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialog2 消息处理程序
void CDialog2::InitGL()
{
	

	glMatrixMode(GL_PROJECTION);
	glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.1f,0.2f,0.3f,1);
	glColor4x(0x10000, 0, 0, 0);
}

void CDialog2::DrawFrame()
{
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
		TRACE("Could not Bind Contexts and Drawables for OpenGL-ES1 display.");
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#if 0
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_SHORT, 0, mVertices);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);
	glDisableClientState(GL_VERTEX_ARRAY);
#else
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
#endif
	glRotatex(2<<16, 0, 0, 0x10000);
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
	//eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}


int CDialog2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_hWnd=this->GetSafeHwnd();
	if(m_hWnd==NULL)
	{
		AfxMessageBox(TEXT("获取渲染区窗口对象时HWND为空"));
		return -1;
	}
	TRACE("2m_hWnd:%d\n",m_hWnd);
	m_hDc = ::GetDC(m_hWnd);
	//m_eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	m_eglDisplay = eglGetDisplay(m_hDc);
	EGLint majorVersion, minorVersion;
	if (!eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		::MessageBox(NULL,TEXT("Could not initialize OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	else
	{
		printf("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}
	//eglBindAPI(EGL_OPENGL_ES_API); 
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
		TRACE( "(error %s)\n", getEGLError(eglGetError()) );
		::MessageBox(NULL,TEXT("2Could not get config for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
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
		TRACE( "(error %s)\n", getEGLError(eglGetError()) );
		::MessageBox(NULL,TEXT("2Could not create surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);
	//m_eglContext = eglCreateContext(m_eglDisplay, config, m_eglContext, eglAttributes);
	if(m_eglContext==EGL_NO_CONTEXT){
		TRACE( "(error %s)\n", getEGLError(eglGetError()) );
		::MessageBox(NULL,TEXT("2Could not create EGLContext for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
		::MessageBox(NULL,TEXT("2Could not Bind Contexts and Drawables for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		return -1;
	}

	InitGL();

	pTexture=NULL;
	pTexture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\24ES2ShadowMapping\\wood.png");

	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。

	return 0;
}

void CDialog2::OnDestroy()
{
	TRACE("2OnDestroy start\n");
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(pTexture)
		delete pTexture;


	KillTimer(WM_RENDER_FRAME);
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

	TRACE("2OnDestroy end\n");
}

BOOL CDialog2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CDialog2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DrawFrame();
}

void CDialog2::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

void CDialog2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//更新窗口整个客户区域，同时重画时不擦除背景。
		InvalidateRect(NULL,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialog2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
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
