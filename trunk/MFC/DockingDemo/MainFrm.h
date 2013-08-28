// MainFrm.h : CMainFrame 类的接口
//


#pragma once

#include "ChildView.h"
struct MarkersLayoutC : public DockingMarkers::LayoutPane
{	MarkersLayoutC() : LayoutPane(112,112,		// total size of marker's group.
	DockingMarkers::MarkerPane( CPoint(4,40),IDB_BITMAP_LEFT,CRect(0,0,32,32) ),		// left marker.
	DockingMarkers::MarkerPane( CPoint(40,4),IDB_BITMAP_TOP,CRect(0,0,32,32) ),			// top marker.
	DockingMarkers::MarkerPane( CPoint(76,40),IDB_BITMAP_RIGHT,CRect(0,0,32,32) ),		// right marker.
	DockingMarkers::MarkerPane( CPoint(40,76),IDB_BITMAP_BOTTOM,CRect(0,0,32,32) ),	// bottom marker.
	DockingMarkers::MarkerPane( CPoint(0,0),IDB_BITMAP_TABS,CRect(36,36,76,76) ),		// marker of tabs.
	RGB(255,0,255))	// color of mask (pixels which don't show).
{
}
};
class CMainFrame : public CFrameWnd,public MultiPaneCtrlUserAbility, public MultiPaneCtrlNotify
{
private: // TabCtrlNotify.
	// MultiPaneCtrlUserAbility.
	virtual bool CanDrop(MultiPaneCtrl *pCtrl, HTAB hTabSrc, DOCKSIDE side, HPANE hPaneDst);
	// 
	// MultiPaneCtrlNotify.
	virtual void OnTabCloseButtonClicked(MultiPaneCtrl *pPaneCtrl, TabCtrl *pTabCtrl, CRect const *pRect, CPoint ptScr);
	virtual void OnTabMenuButtonClicked(MultiPaneCtrl *pPaneCtrl, TabCtrl *pTabCtrl, CRect const *pRect, CPoint ptScr);
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
	//DemoDlg m_TestDlg;
	CDialog1 m_dlg1;
	//CFormView1* m_fv1;


// 生成的消息映射函数
protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	/*afx_msg void OnUpdateTestdlg(CCmdUI *pCmdUI);
	afx_msg void OnTestdlg();
	afx_msg void OnUpdateLoadstateState1(CCmdUI *pCmdUI);
	afx_msg void OnLoadstateState1();
	afx_msg void OnUpdateLoadstateState2(CCmdUI *pCmdUI);
	afx_msg void OnLoadstateState2();
	afx_msg void OnUpdateLoadstateState3(CCmdUI *pCmdUI);
	afx_msg void OnLoadstateState3();
	afx_msg void OnUpdateSavestateState1(CCmdUI *pCmdUI);
	afx_msg void OnSavestateState1();
	afx_msg void OnUpdateSavestateState2(CCmdUI *pCmdUI);
	afx_msg void OnSavestateState2();
	afx_msg void OnUpdateSavestateState3(CCmdUI *pCmdUI);
	afx_msg void OnSavestateState3();
	afx_msg void OnUpdateShowclientedgeborder(CCmdUI *pCmdUI);
	afx_msg void OnShowclientedgeborder();*/
public:
	afx_msg void On32771();
	afx_msg void OnUpdate32771(CCmdUI *pCmdUI);
};


