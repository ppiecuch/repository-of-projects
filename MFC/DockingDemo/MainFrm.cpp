// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DockingDemo.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()

	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// 
	/*ON_UPDATE_COMMAND_UI(ID_TESTDLG, OnUpdateTestdlg)
	ON_COMMAND(ID_TESTDLG, OnTestdlg)
	// 
	ON_UPDATE_COMMAND_UI(ID_LOADSTATE_STATE1, CMainFrame::OnUpdateLoadstateState1)
	ON_COMMAND(ID_LOADSTATE_STATE1, CMainFrame::OnLoadstateState1)
	ON_UPDATE_COMMAND_UI(ID_LOADSTATE_STATE2, CMainFrame::OnUpdateLoadstateState2)
	ON_COMMAND(ID_LOADSTATE_STATE2, CMainFrame::OnLoadstateState2)
	ON_UPDATE_COMMAND_UI(ID_LOADSTATE_STATE3, CMainFrame::OnUpdateLoadstateState3)
	ON_COMMAND(ID_LOADSTATE_STATE3, CMainFrame::OnLoadstateState3)
	ON_UPDATE_COMMAND_UI(ID_SAVESTATE_STATE1, CMainFrame::OnUpdateSavestateState1)
	ON_COMMAND(ID_SAVESTATE_STATE1, CMainFrame::OnSavestateState1)
	ON_UPDATE_COMMAND_UI(ID_SAVESTATE_STATE2, CMainFrame::OnUpdateSavestateState2)
	ON_COMMAND(ID_SAVESTATE_STATE2, CMainFrame::OnSavestateState2)
	ON_UPDATE_COMMAND_UI(ID_SAVESTATE_STATE3, CMainFrame::OnUpdateSavestateState3)
	ON_COMMAND(ID_SAVESTATE_STATE3, CMainFrame::OnSavestateState3)
	ON_UPDATE_COMMAND_UI(ID_SHOWCLIENTEDGEBORDER, CMainFrame::OnUpdateShowclientedgeborder)
	ON_COMMAND(ID_SHOWCLIENTEDGEBORDER, CMainFrame::OnShowclientedgeborder)*/
	ON_COMMAND(ID_32771, &CMainFrame::On32771)
	ON_UPDATE_COMMAND_UI(ID_32771, &CMainFrame::OnUpdate32771)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建一个视图以占用框架的工作区
	/*if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}*/
	


	// Creation and initialization of child windows.
	if(m_Tree1.Create(WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | 
		TVS_HASLINES,CRect(0,0,0,0),this,300)==0 ||
		m_Tree2.Create(WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | 
		TVS_HASLINES,CRect(0,0,0,0),this,301)==0 ||
		m_Edit1.Create(WS_CHILD | ES_MULTILINE,CRect(0,0,0,0),this,302)==0 ||
		m_List1.Create(WS_CHILD | LVS_REPORT,CRect(0,0,0,0),this,303)==0 ||
		m_List2.Create(WS_CHILD | LVS_REPORT,CRect(0,0,0,0),this,304)==0 
		//||m_List3.Create(WS_CHILD | LVS_REPORT,CRect(0,0,0,0),this,305)==0
		)
		return -1;

	m_Tree1.InsertItem(_T("CTreeCtrl 1"));
	m_Tree2.InsertItem(_T("CTreeCtrl 2"));
	m_Edit1.SetWindowText(_T("CEdit 1"));
	m_List1.InsertColumn(0,_T("CListCtrl 1"),LVCFMT_LEFT,100);
	m_List1.InsertItem(0,_T("Item 1"));
	m_List2.InsertColumn(0,_T("CListCtrl 2"),LVCFMT_LEFT,100);
	m_List2.InsertItem(0,_T("Item 1"));
	//m_List3.InsertColumn(0,_T("CListCtrl 3"),LVCFMT_LEFT,100);
	//m_List3.InsertItem(0,_T("Item 1"));

	if(m_dlg1.Create(CDialog1::IDD,this)==0) return -1;	// create as modeless dialog box.
	m_dlg1.SetDlgCtrlID(CDialog1::IDD);		// set unique id - important for dialog box.

	// Creation of MultiPaneCtrl object.
	if(m_MultiPaneCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),AFX_IDW_PANE_FIRST)==false) return -1;

	CImageList imagelist,imagelistDis, imagelistSys;
	CBitmap bmp,bmpDis, bmpSys;

	imagelist.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmp.LoadBitmap(IDB_BITMAP1);
	imagelist.Add(&bmp,RGB(255,0,255));

	imagelistDis.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmpDis.LoadBitmap(IDB_BITMAP2);
	imagelistDis.Add(&bmpDis,RGB(255,0,255));

	imagelistSys.Create(14,14,ILC_COLOR24 | ILC_MASK,7,0);
	bmpSys.LoadBitmap(IDB_BITMAP3);
	imagelistSys.Add(&bmpSys,RGB(255,0,255));

	m_MultiPaneCtrl.SetImageLists(&imagelist,&imagelistDis);
	m_MultiPaneCtrl.SetSystemImageList(&imagelistSys);

	m_MultiPaneCtrl.SetNotifyManager(this);
	m_MultiPaneCtrl.SetCursors(IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3,IDC_CURSOR4,IDC_CURSOR5);

	m_MultiPaneCtrl.SetDockMarkers(MarkersLayoutC(),DockingMarkers::Params(40,true,14),50);
	m_MultiPaneCtrl.RemoveTabEnable(true);
	m_MultiPaneCtrl.DragTabEnable(true);

	// Loading state or creation default state.
	try
	{
		
		tabs.Add(m_Tree1,_T("Tree1"),-1);
		tabs.Add(m_List1,_T("List1"),0);
		tabs.Add(m_Edit1,_T("Edit1"),1);
		tabs.Add(m_List2,_T("List2"),2);
		tabs.Add(m_Tree2,_T("Tree2"),3);
		tabs.Add(m_dlg1,_T("Dtlg1"),4);
		//tabs.Add(m_List3,_T("List3"),4);

		// 
		if(m_MultiPaneCtrl.LoadState(AfxGetApp(),_T("MultiPaneCtrl"), _T("State"),&tabs,false)==false)
		{
			// create default state.
			HPANE h1 = m_MultiPaneCtrl.ConvertToLine(NULL,false);
			m_MultiPaneCtrl.AddTab(h1,tabs[0]);
			m_MultiPaneCtrl.AddTab(h1,tabs[1]);

			HPANE h2 = m_MultiPaneCtrl.Add(NULL);

			HPANE h3 = m_MultiPaneCtrl.ConvertToLine(h2,true);
			m_MultiPaneCtrl.AddTab(h3,tabs[2]);

			HPANE h4 = m_MultiPaneCtrl.Add(h2);
			HPANE h5 = m_MultiPaneCtrl.ConvertToLine(h4,false);
			m_MultiPaneCtrl.AddTab(h5,tabs[3]);

			HPANE h6 = m_MultiPaneCtrl.Add(h4);
			m_MultiPaneCtrl.AddTab(h6,tabs[4]);

			HPANE h7 = m_MultiPaneCtrl.Add(h2);
			HPANE h8 = m_MultiPaneCtrl.ConvertToLine(h7,false);
			m_MultiPaneCtrl.AddTab(h8,tabs[5]);

			m_MultiPaneCtrl.SetEqualPaneSize();
		}
		m_MultiPaneCtrl.ShowTabMenuButton(false);
		m_MultiPaneCtrl.ShowTabCloseButton(false);
	}
	catch(std::bad_alloc &)
	{    return -1;
	}

	m_MultiPaneCtrl.Update();


	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	//m_wndView.SetFocus();
	//m_MultiPaneCtrl.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	if (m_MultiPaneCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnDestroy()
{	m_MultiPaneCtrl.SaveState(AfxGetApp(),_T("MultiPaneCtrl"),_T("State"));
// 
CFrameWnd::OnDestroy();
}
bool CMainFrame::CanDrop(MultiPaneCtrl * /*pCtrl*/, HTAB /*hTabSrc*/, DOCKSIDE side, HPANE /*hPaneDst*/)
{		
	return true;
}
void CMainFrame::OnTabCloseButtonClicked(MultiPaneCtrl* pPaneCtrl, TabCtrl* pTabCtrl, CRect const *pRect, CPoint ptScr)
{
	
	//pPaneCtrl->GetWindowText(str);
	//::MessageBox(m_hWnd,_T("DemoDlg::OnTabCloseButtonClicked"),str,MB_OK);
	//m_MultiPaneCtrl.HideSingleTab(true);
	//m_MultiPaneCtrl.HitTest(ptScr);
	
	//pTabCtrl->HideSingleTab(true);
	//m_dlg1.ShowWindow(SW_HIDE);
	//m_MultiPaneCtrl.RedrawWindow(0,0,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	//临时解决方案：因为删除后无法重新布局，所以限制只有在List中包含两个或两个以上TAB时才能删除
	//==>发现隐藏后无法恢复，屏蔽隐藏功能
// 	if(pTabCtrl->GetCount()>1)
// 	{
// 		pTabCtrl->Delete(pTabCtrl->GetSel());
// 		m_MultiPaneCtrl.Update();
// 	}
	
	
}
// 
void CMainFrame::OnTabMenuButtonClicked(MultiPaneCtrl * /*pPaneCtrl*/, TabCtrl * /*pTabCtrl*/, CRect const * /*pRect*/, CPoint /*ptScr*/)
{	::MessageBox(m_hWnd,_T("DemoDlg::OnTabMenuButtonClicked"),_T("DemoDlg"),MB_OK);
}
void CMainFrame::On32771()
{
	// TODO: 在此添加命令处理程序代码
	
}

void CMainFrame::OnUpdate32771(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//TRACE("%d\r\n",tabs.GetCount());
	//pCmdUI->SetCheck(m_MultiPaneCtrl.GetFocus()->IsWindowVisible());
}
