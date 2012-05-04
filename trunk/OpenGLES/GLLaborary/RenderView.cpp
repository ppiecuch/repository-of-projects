// RenderView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GLLaborary.h"
#include "RenderView.h"


// CRenderView
extern HGLRC g_pRenderRC;
extern CDC * g_pRenderDC;

extern HGLRC g_pTopRC;
extern CDC * g_pTopDC;

IMPLEMENT_DYNCREATE(CRenderView, CFormView)

CRenderView::CRenderView()
: CFormView(CRenderView::IDD)
{
	m_xpos = 0.0f;
	m_ypos = 0.0f;
	m_zoom = 20.0f;
	m_xrot = 0.0f;
	m_yrot = 0.0f;
}

CRenderView::~CRenderView()
{
}

void CRenderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRenderView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRenderView ���

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRenderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRenderView ��Ϣ�������

void CRenderView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (!::wglMakeCurrent(g_pRenderDC->GetSafeHdc(),g_pRenderRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return ;
	}
	m_drawObject.PrepareSetting(m_xpos, m_ypos, m_xrot, m_yrot, m_zoom);
	m_drawObject.DrawRenderScene();

	::wglMakeCurrent(g_pRenderDC->GetSafeHdc(),NULL);
}

BOOL CRenderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;

	return CFormView::PreCreateWindow(cs);
}

int CRenderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialization
	InitOpenGL();

	if (!::wglMakeCurrent(g_pRenderDC->GetSafeHdc(),g_pRenderRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return FALSE;
	}
	m_drawObject.SpecialEffect();
	::wglMakeCurrent(NULL, NULL);

	//����ˢ��ʱ��
	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣
	return 0;	
}

void CRenderView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(cy > 0)
	{

		if (!::wglMakeCurrent(g_pRenderDC->GetSafeHdc(),g_pRenderRC)){
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

BOOL CRenderView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CFormView::OnEraseBkgnd(pDC);
	return TRUE;
}

void CRenderView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (::wglGetCurrentContext() != NULL) {
		::wglMakeCurrent(NULL,  NULL);
	}

	if(g_pRenderRC){
		::wglDeleteContext(g_pRenderRC);
		g_pRenderRC = NULL;
	}
	if(g_pRenderDC){
		delete g_pRenderDC;
		g_pRenderDC = NULL;
	}

	KillTimer(WM_RENDER_FRAME);
}
BOOL CRenderView::bSetupPixelFormat(HDC hdc)
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
BOOL CRenderView::InitOpenGL()
{
	g_pRenderDC = new CClientDC(this);

	ASSERT(g_pRenderDC != NULL);

	if(!bSetupPixelFormat(g_pRenderDC->GetSafeHdc()))
		return FALSE;

	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormatIndex;
	nPixelFormatIndex = ::GetPixelFormat(::wglGetCurrentDC());
	::DescribePixelFormat(::wglGetCurrentDC(), nPixelFormatIndex, sizeof(pfd), &pfd);

	g_pRenderRC = ::wglCreateContext(g_pRenderDC->GetSafeHdc()); 
	if (!::wglMakeCurrent(g_pRenderDC->GetSafeHdc(),g_pRenderRC)){
		AfxMessageBox(_T("MakeCurrentError"));
		return FALSE;
	}

	GetClientRect(&m_oldRect);

	m_drawObject.SetView(this, 0);		// Set view project model	

	//TODO�Ƶ�������
	int main_version, sub_version, release_version;
	const char* version = (const char*)glGetString(GL_VERSION);
	const char* vendor=(const char*)glGetString(GL_VENDOR);
	const char* renderer=(const char*)glGetString(GL_RENDERER);
	sscanf(version, "%d.%d.%d", &main_version, &sub_version, &release_version);
	TRACE("OpenGL �汾��%s\n", version);
	TRACE("���汾�ţ�%d\n", main_version);
	TRACE("�ΰ汾�ţ�%d\n", sub_version);
	TRACE("���а汾�ţ�%d\n", release_version);
	TRACE("OpenGL�ṩ���̣�%s\n",vendor);
	TRACE("�Կ����ƣ�%s\n",renderer);

	int maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&maxTextureUnits);
	TRACE("֧���������λ��%d\n",maxTextureUnits);

	int maxTextureImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxTextureImageUnits);
	TRACE("֧����������ͼ��Ԫ��%d\n",maxTextureImageUnits);

	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxTextureSize);
	TRACE("֧����������ͼ���С��%d\n",maxTextureSize);

	int maxLights;
	glGetIntegerv(GL_MAX_LIGHTS,&maxLights);
	TRACE("֧�ֹ�Դ���ֵ��%d\n",maxLights);


	::wglMakeCurrent(NULL, NULL);
	m_drawObject.SetRC();		// Set Render contex -- current is lighting option
	return TRUE;
}
BOOL CRenderView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
void CRenderView::OnMouseMove(UINT nFlags, CPoint point) 
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
void CRenderView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//���´��������ͻ�����ͬʱ�ػ�ʱ������������
		InvalidateRect(NULL,FALSE);
	}

	CFormView::OnTimer(nIDEvent);
}
