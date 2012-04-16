// YonExampleDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "YonExampleDlg.h"
#include "YonExampleDlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CYonExampleDlgDlg 对话框




CYonExampleDlgDlg::CYonExampleDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYonExampleDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYonExampleDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYonExampleDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CYonExampleDlgDlg 消息处理程序

BOOL CYonExampleDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HWND hWnd=this->GetSafeHwnd();
	if(hWnd==NULL)
	{
		AfxMessageBox(TEXT("获取渲染区窗口对象时HWND为空"));
		return -1;
	}

	SYonEngineParameters params;
	params.windowId=hWnd;
	m_hWnd=hWnd;
	engine=CreateEngine(params);

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();

	initEGL(hWnd);
	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CYonExampleDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYonExampleDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		if(engine->run())
		{
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CYonExampleDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYonExampleDlgDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(WM_RENDER_FRAME);
	Logger->debug("CYonExampleMFCView::OnDestroy\n");
	destroyEGL();
	engine->drop();
}

BOOL CYonExampleDlgDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CYonExampleDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//更新窗口整个客户区域，同时重画时不擦除背景。
		InvalidateRect(NULL,FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}
bool CYonExampleDlgDlg::initEGL(const HWND& hwnd){

	EGLConfig config;
	EGLint num_configs;
	m_hDc = ::GetDC(hwnd);

	//First Step：Get EGLDisplay Object
	//The type and format of display_id are implementation-specific,
	//and it describes a specific display provided by the system EGL is running on.
	//For example, an EGL implementation under Microsoft Windows could define 
	//display_id to be a Windows Device Context.
	//If no display matching display_id is available,EGL_NO_DISPLAY is returned;
	//no error condition is raised in this case.
	m_eglDisplay = eglGetDisplay(m_hDc);
	/*if(m_eglDisplay == EGL_NO_DISPLAY)
	{
	m_eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	Logger->info("eglGetDisplay(EGL_DEFAULT_DISPLAY)\n");
	}*/

	//Second Step:Initialize EglDisplay Object
	//EGL_TRUE is returned on success, and major and minor are updated with
	// the major and minor version numbers of the EGL implementation.major 
	//and minor are not updated if they are specified as NULL.
	//EGL_FALSE is returned on failure and major and minor are not updated.
	//An EGL_BAD_DISPLAY error is generated if the dpy argument does not refer
	//to a valid EGLDisplay.An EGL_NOT_INITIALIZED error is generated if EGL cannnot be 
	//initialized for an otherwise valid dpy.
	//Initializing an already-initialized display is allowed,but the only effect of such 
	//a call is to return EGL_TRUE and update the EGL version numbers.
	EGLint majorVersion, minorVersion;
	if (!eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
	{
		//MessageBox(NULL,TEXT("Could not initialize OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		Logger->info(YON_LOG_FAILED_FORMAT,"Initialize EglDisplay Object");
		return false;
	}
	else
	{
		Logger->info("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
	}

	//EGL_RED_SIZE: bits of red in the color buffer
	//EGL_GREEN_SIZE: ...
	//EGL_BLUE_SIZE: ...
	//EGL_ALPHA_SIZE: ...
	//EGL_BUFFER_SIZE: total color component bits in the color buffer
	//EGL_SURFACE_TYPE: a mask indicating capabilities of surfaces that
	//				can be created with the corresponding EGLConfig(Default:EGL_WINDOW_BIT)
	//EGL_DEPTH_SIZE: bits of Z in the depth buffer
	//EGL_STENCIL_SIZE: bits of Stencil in the stencil buffer
	//EGL_SAMPLE_BUFFERS: number of multisample buffers
	//EGL_RENDERABLE_TYPE: a mask indicating which client APIs can render into a surface
	//				created with respect to and EGLConfig.
	//Creation of a client API context based on an EGLConfig will fail unless the EGLConfig's
	//EGL_RENDERABLE_TYPE attribute include the bit corresponding to that API and version.
	//				(Default:EGL_OPENGL_ES_BIT)
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

	//Third Step: Choose EGLConfig
	//API: EGLBoolean eglChooseConfig(EGLDisplay dpy,const EGLint *attrib_list,
	//			EGLConfig* configs,EGLint config_size, EGLint *num_config);
	//If no EGLConfig matching the attribute list exists, then the call
	//succeeds,but num_configs is set to 0.
	//If more than one matching EGLConfig is found,then a list of EGLConfigs is returned.
	//In most cases we just want the first config that meets all criteria, so we limit the
	//number of configs returned to 1.
	if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
	{
		//MessageBox(NULL,TEXT("Could not get config for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		Logger->info(YON_LOG_FAILED_FORMAT,"Choose EGLConfig");
		return false;
	}

	//Fourth Step: Create EGLSurface
	//EGLSurface eglCreateWindowSurface(EGLDisplay dpy,EGLConfig config,
	//			EGLNativeWindowType win,const EGLint *attrib_list)
	//eglCreateWindowSurface creates an onscreen EGLSurface and returns a handle to it.
	//Any EGL context created with a compatible EGLConfig can be used to render into this
	//surface.
	//when the attrib_list is NULL,all attributes use default value as below:
	//EGL_RENDER_BUFFER: EGL_BACK_BUFFER
	//On failure eglCrateWindowSurface returns EGL_NO_SURFACE
	//Use eglMakeCurrent to attach an EGL rendering context to the surface
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, hwnd, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
		//why?
		//Through platform specific functions we need to create a window that we can
		//use for OpenGL ES output.If there is no window system win should remain 0.
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
	if (EGL_NO_SURFACE==m_eglSurface)
	{
		//MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
		Logger->info(YON_LOG_FAILED_FORMAT,"Create EGLSurface");
		return false;
	}

	//eglBindAPI(EGL_OPENGL_ES_API);

	//The list is terminated with EGL_NONE.
	//If an attribute is not specified in eglAttributes,then the default value is used.
	//If EGL_DONT_CARE is specified as an attribute value,then the attribute will not
	//be checked.
	//EGL_DONT_CARE may be specified for all attributes except EGL_LEVEL.
	EGLint eglAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};

	//Fifth Step: Create EGLContext
	//EGL has to create a context for OpenGL ES.Our OpenGL ES resources like textures will only be valid inside this context(or shared contexs).
	//API: EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
	//If eglCreateContext succeeds,it initializes the context to the initial state defined for the current
	//rendering API, and returns a handle to it.The context can be used to render to any compatible EGLSurface.
	//On failure eglCreateContext returns EGL_NO_CONTEXT.If the current rendering api is EGL_NONE,then an EGL_BAD_MATCH error is
	//generated(this situation can only arise in an implementation which does not support OpenGL ES,and prior to the first call to
	//eglBindAPI).
	//attrib_list specifies a list of attributes for the context.The list has the same structure as described for eglChooseConfig.
	//The only attribute that can be specified in attrib_list is EGL_CONTEXT_CLIENT_VERSION, and this attribute may only be
	//specified when creating a OpenGL ES context
	//EGL_CONTEXT_CLIENT_VERSION determines which version of an OpenGL ES context to create.An attribute value of 1 specifies creation
	//of an OpenGL ES 1.x context.An attribute value of 2 specifies creation of an OpenGL ES 2.x context.(Default:1)
	m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);
	//m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, NULL);

	//TODO check error

	//Sixth Step: Bind Contexts and Drawables
	//API: EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
	//eglMakeCurrent binds ctx to the current rendering thread and to the draw and read surfaces
	//For an OpenGL and OpenGL ES context,draw is used for all operations except for any pixel data read
	//back or copied,which is taken from the frame buffer values of read.
	//eglMakeCurrent returns EGL_FALSE on failure.
	if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
		Logger->info(YON_LOG_FAILED_FORMAT,"Bind Contexts and Drawables");
		return false;
	}
	return true;
}

void CYonExampleDlgDlg::destroyEGL(){
	//Finial Step: Terminate OpenGL ES and destroy the window(if present).
	//eglTerminate takes care of destroying any context or surface created with this display,so we don't need to call
	//eglDestroySurface or eglDestroyContext here.
	//To release the current context without assigning a new one, set ctx to EGL_NO_CONTEXT and set draw and read to
	//EGL_NO_SURFACE.
	if(eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)==EGL_TRUE){
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Unbind GL contexts and surface");
	}else{
		Logger->info(YON_LOG_FAILED_FORMAT,"Unbind GL contexts and surface");
	}
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglDestroySurface(m_eglDisplay, m_eglSurface);

	//To release resources associated with use of EGL and client APIs on a display, call
	//EGLBoolean eglTerminate(EGLDisplay dpy);
	//Termination marks all EGL-specific resources,such as contexts and surfaces,associated with the specified display for
	//deletion.Handles to all such resources are invalid as soon as eglTerminate returns,but the dpy handle itself remains
	//valid.
	//eglTerminate returns EGL_TRUE on success.
	if(eglTerminate(m_eglDisplay)==EGL_TRUE){
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release all resources in EGL and display");
	}else{
		Logger->info(YON_LOG_FAILED_FORMAT,"Release all resources in EGL and display");
	}
	if (m_hDc&&::ReleaseDC((HWND)m_hWnd, m_hDc)){
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release DC");
	}else{
		Logger->info(YON_LOG_FAILED_FORMAT,"Release DC");
	}
}