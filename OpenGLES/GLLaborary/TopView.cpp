// TopView.cpp : implementation file
//

#include "stdafx.h"
#include "TopView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HGLRC g_pRenderRC;
extern CDC * g_pRenderDC;

extern HGLRC g_pTopRC;
extern CDC * g_pTopDC;
/////////////////////////////////////////////////////////////////////////////
// CTopView

IMPLEMENT_DYNCREATE(CTopView, CFormView)

CTopView::CTopView(): CFormView(CTopView::IDD)
{
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_zoom = 20.0f;
	m_xrot = 0.0f;
	m_yrot = 0.0f;
}

CTopView::~CTopView()
{
}
void CTopView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTopView, CFormView)
	//{{AFX_MSG_MAP(CTopView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()

	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopView drawing

void CTopView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	if (!::wglMakeCurrent(g_pTopDC->GetSafeHdc(),g_pTopRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return ;
	}
	m_drawObject.PrepareSetting(m_xpos, m_ypos, m_xrot, m_yrot, m_zoom);
	m_drawObject.DrawTestScene();

	::wglMakeCurrent(g_pTopDC->GetSafeHdc(),NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CTopView diagnostics

#ifdef _DEBUG
void CTopView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTopView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTopView message handlers

BOOL CTopView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;

	return CFormView::PreCreateWindow(cs);
}

BOOL CTopView::bSetupPixelFormat(HDC hdc)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		32,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		8,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer	 
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	};
	int nGLPixelIndex;
	if((nGLPixelIndex = ::ChoosePixelFormat(hdc, &pfd)) == 0){
		AfxMessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if(::SetPixelFormat(hdc, nGLPixelIndex, &pfd) == FALSE){
		AfxMessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CTopView::InitOpenGL()
{
	g_pTopDC = new CClientDC(this);

	ASSERT(g_pTopDC != NULL);

	if(!bSetupPixelFormat(g_pTopDC->GetSafeHdc()))
		return FALSE;

	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormatIndex;
	nPixelFormatIndex = ::GetPixelFormat(::wglGetCurrentDC());
	::DescribePixelFormat(::wglGetCurrentDC(), nPixelFormatIndex, sizeof(pfd), &pfd);

	g_pTopRC = ::wglCreateContext(g_pTopDC->GetSafeHdc()); 
	if (!::wglMakeCurrent(g_pTopDC->GetSafeHdc(),g_pTopRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return FALSE;
	}

	GetClientRect(&m_oldRect);

	m_drawObject.SetView(this, 0);		// Set view project model	


	glewInit();
	GLEnvironment::check();
	GLEnvironment::setShaders();
	

	::wglMakeCurrent(NULL, NULL);
	m_drawObject.SetRC();		// Set Render contex -- current is lighting option
	return TRUE;
}

int CTopView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialization
	InitOpenGL();

	if (!::wglMakeCurrent(g_pTopDC->GetSafeHdc(),g_pTopRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return FALSE;
	}
	m_drawObject.SpecialEffect();
	::wglMakeCurrent(NULL, NULL);

	//定义刷新时间
	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。
	return 0;
}

void CTopView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	if(cy > 0)
	{

		if (!::wglMakeCurrent(g_pTopDC->GetSafeHdc(),g_pTopRC)){
			AfxMessageBox(_T("MakeCurrentError"));
			return ;
		}

		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;


		m_drawObject.SetView(this, 0);
		glViewport(0, 0, cx, cy);

		::wglMakeCurrent(NULL, NULL);
	}
}

BOOL CTopView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CTopView::OnDestroy() 
{
	CFormView::OnDestroy();

	if (::wglGetCurrentContext() != NULL) {
		::wglMakeCurrent(NULL,  NULL);
	}

	if(g_pTopRC){
		::wglDeleteContext(g_pTopRC);
		g_pTopRC = NULL;
	}
	if(g_pTopDC){
		delete g_pTopDC;
		g_pTopDC = NULL;
	}

	KillTimer(WM_RENDER_FRAME);

}
BOOL CTopView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
void CTopView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//更新窗口整个客户区域，同时重画时不擦除背景。
		InvalidateRect(NULL,FALSE);
	}

	CFormView::OnTimer(nIDEvent);
}

void CTopView::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags & MK_LBUTTON )
	{
		if( nFlags & MK_SHIFT )
		{
			// SHIFT is being
			// pressed. Zoom the camera.
			if ( m_lastMouseY != -1 )
			{
				m_zoom += (point.y - m_lastMouseY);
				// Redraw the viewport.
				OnDraw( NULL );
			}
			m_lastMouseY = point.y;
		}
		else if ( nFlags & MK_CONTROL )
		{
			// CONTROL is being
			// pressed. Pan the camera.
			if ( m_lastMouseX != -1 )
			{
				m_xpos += (point.x - m_lastMouseX) * 0.2;
				m_ypos += (point.y - m_lastMouseY) * 0.2;
				// Redraw the viewport.
				OnDraw( NULL );
			}
			m_lastMouseX = point.x;
			m_lastMouseY = point.y;
		}
		else
		{
			// Left mouse button is being
			// pressed. Rotate the camera.
			if ( m_lastMouseX != -1 )
			{
				m_xrot += point.y - m_lastMouseY;
				m_yrot += point.x - m_lastMouseX;
				// Redraw the viewport.
				OnDraw( NULL );
			}
			m_lastMouseX = point.x;
			m_lastMouseY = point.y;
		}
	}
	else
	{
		m_lastMouseX = -1;
		m_lastMouseY = -1;
	}	


	CFormView::OnMouseMove(nFlags, point);
}
