// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "stdafx.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	CSizingControlBarG m_wndMyBar1;///���ǽ�����Ϊ��ߵĴ���
	CSizingControlBarG m_wndMyBar2;///ͣ�����·��Ĵ���

	CEdit	  m_wndEdit;

	CDialog1 dlg1;
	CDialog2 dlg2;

	bool b;
public:
	afx_msg void On32771();
	afx_msg void OnUpdate32772(CCmdUI *pCmdUI);
	afx_msg void On32772();
};


