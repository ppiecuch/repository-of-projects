// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "stdafx.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	CSizingControlBarG m_wndMyBar1;///我们将它作为左边的窗口
	CSizingControlBarG m_wndMyBar2;///停靠在下方的窗口

	CEdit	  m_wndEdit;

	CDialog1 dlg1;
	CDialog2 dlg2;

	bool b;
public:
	afx_msg void On32771();
	afx_msg void OnUpdate32772(CCmdUI *pCmdUI);
	afx_msg void On32772();
};


