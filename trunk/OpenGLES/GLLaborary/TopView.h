#pragma once



// CTopView ������ͼ

class CTopView : public CFormView
{
	DECLARE_DYNCREATE(CTopView)

protected:
	
	virtual ~CTopView();

	const static UINT WM_RENDER_FRAME=2;//��Ⱦ�¼���Ϣ,��Timerʹ��
	const static UINT RENDER_INTERVAL=50;//��ȾƵ��
	const static unsigned long DOWNLOAD_TIMELEN_LIMIT=100;//ǰ�������·���Ϣ�ļ������

public:
	CTopView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	enum { IDD = IDD_FORMVIEW_TOP };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	

	BOOL InitOpenGL();
	BOOL bSetupPixelFormat(HDC hdc);

	CDrawing	m_drawObject;
	CRect		m_oldRect;
	GLfloat		m_ratio;
	GLfloat		m_zoom,m_xpos,m_ypos,m_xrot,m_yrot;
	GLint	m_lastMouseX,m_lastMouseY;
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


