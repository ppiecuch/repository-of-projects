// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "stdafx.h"
#include "ChildView.h"

struct MarkersLayoutC : public DockingMarkers::LayoutPane
{	MarkersLayoutC() : LayoutPane(112,112,		// total size of marker's group.
		DockingMarkers::MarkerPane( CPoint(4,40),IDB_BITMAP_LEFT,CRect(0,0,32,32) ),		// left marker.
		DockingMarkers::MarkerPane( CPoint(40,4),IDB_BITMAP_TOP,CRect(0,0,32,32) ),			// top marker.
		DockingMarkers::MarkerPane( CPoint(76,40),IDB_BITMAP_RIGHT,CRect(0,0,32,32) ),		// right marker.
		DockingMarkers::MarkerPane( CPoint(40,76),IDB_BITMAP_BOTTOM,CRect(0,0,32,32) ),	// bottom marker.
		DockingMarkers::MarkerPane( CPoint(0,0),IDB_BITMAP_TABS,CRect(36,36,76,76) ),		// marker of tabs.
		RGB(255,0,255)){}	// color of mask (pixels which don't show).
};
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

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
	CChildView    m_wndView;

	CTreeCtrl m_Tree1, m_Tree2;
	CEdit m_Edit1;
	CListCtrl m_List1, m_List2, m_List3;
	// 
	MultiPaneCtrlEx<MultiPaneCtrlStyle_VS2003_client> m_MultiPaneCtrl;
	MultiPaneCtrl::Tabs tabs;
	//CRenderDialog renderDlg;
	//CMiniatureDialog miniatureDlg;
	CRenderView* m_pRenderView;
	CTopView* m_pTopView;
	//CListView *pListView;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};


