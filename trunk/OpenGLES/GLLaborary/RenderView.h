#pragma once

#include "stdafx.h"

// CRenderView 窗体视图

class CRenderView : public CFormView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	
	virtual ~CRenderView();

	const static UINT WM_RENDER_FRAME=1;//渲染事件消息,由Timer使用
	const static UINT RENDER_INTERVAL=50;//渲染频率
	const static unsigned long DOWNLOAD_TIMELEN_LIMIT=100;//前后两条下发消息的间隔界限

public:
	CRenderView();           // 动态创建所使用的受保护的构造函数
	enum { IDD = IDD_FORMVIEW_RENDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	BOOL InitOpenGL();
	BOOL bSetupPixelFormat(HDC hdc);

	CDrawing	m_drawObject;
	CRect		m_oldRect;
	GLfloat		m_ratio;
	GLfloat		m_zoom,m_xpos,m_ypos,m_xrot,m_yrot;
	GLint	m_lastMouseX,m_lastMouseY;
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


