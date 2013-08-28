// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "BarDemo.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_32771, &CMainFrame::On32771)
	ON_UPDATE_COMMAND_UI(ID_32772, &CMainFrame::OnUpdate32772)
	ON_COMMAND(ID_32772, &CMainFrame::On32772)
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

	//创建ControlBar 
	if (!m_wndMyBar1.Create(_T("我的控制条"),this,123)) 
	{ TRACE0("Failed to create mybar\n"); return -1; } 
	if (!m_wndMyBar2.Create(_T("我的控制条2"),this,CSize(100,100),TRUE,124))
	{ TRACE0("Failed to create mybar\n"); return -1; }


	//停靠控制
	m_wndMyBar1.SetBarStyle(m_wndMyBar1.GetBarStyle() | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC); 
	m_wndMyBar2.SetBarStyle(m_wndMyBar2.GetBarStyle() | CBRS_TOOLTIPS |
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMyBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMyBar2.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMyBar1, AFX_IDW_DOCKBAR_LEFT);///停靠在左边
	DockControlBar(&m_wndMyBar2, AFX_IDW_DOCKBAR_BOTTOM);///停靠在右边

	//在创建ControlBar后创建编辑控件
	//m_wndEdit.Create(WS_VSCROLL|WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|
	//	ES_MULTILINE|ES_WANTRETURN,CRect(0,0,0,0),&m_wndMyBar2,101);
	//m_wndEdit.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	dlg1.Create(IDD_DIALOG1,&m_wndMyBar1);
	dlg1.ShowWindow(SW_SHOW);

	dlg2.Create(IDD_DIALOG2,&m_wndMyBar2);
	dlg2.ShowWindow(SW_SHOW);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

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




void CMainFrame::On32771()
{
	// TODO: 在此添加命令处理程序代码
	TRACE("fdsafads");
}

void CMainFrame::OnUpdate32772(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(b);
}

void CMainFrame::On32772()
{
	// TODO: 在此添加命令处理程序代码
	b=!b;

	//m_wndMyBar1.GetParentFrame()->ShowWindow(b);
	ShowControlBar(&m_wndMyBar1,b,false);
}
