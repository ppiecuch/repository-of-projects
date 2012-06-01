#pragma once
#include "stdafx.h"

// CDialog1 对话框

class CDialog1 : public CDialog
{
	DECLARE_DYNAMIC(CDialog1)

public:
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;
	CDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialog1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void InitGL();
	void DrawFrame();

	EGLDisplay m_eglDisplay;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;

	
	CTexture* pTexture;

	HDC m_hDc;
	HWND m_hWnd;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
