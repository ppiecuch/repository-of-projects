// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Toolbar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_COMMAND(ID_BUTTON_TEST1, OnButtonTest)
	ON_COMMAND_RANGE(ID_BUTTON_TEST1,ID_BUTTON_TEST3,OnButton)
	ON_COMMAND_RANGE(ID_BUTTON32782,ID_BUTTON32782,OnButton)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BUTTON_TEST1,ID_BUTTON_TEST3,OnUpdateButton)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BUTTON32782,ID_BUTTON32782,OnUpdateButton)
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
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
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

	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar(IDR_TOOLBAR2))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar2);

	m_wndToolBar2.ShowWindow(SW_HIDE);

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



void CMainFrame::OnButton(UINT nID)
{
	m_bIndex=(int)(nID-ID_BUTTON_TEST1);
	swapToolbar();
}

void CMainFrame::OnUpdateButton(CCmdUI* pCmdUI)
{
	//按钮按下检查状态设置函数
	pCmdUI->SetCheck((UINT)(ID_BUTTON_TEST1+m_bIndex) == pCmdUI->m_nID);

}

void CMainFrame::swapToolbar(void)
{
	//m_wndToolBar.ShowWindow(SW_HIDE);
	//m_wndToolBar2.ShowWindow(SW_SHOW);
	if(m_wndToolBar.IsVisible())
	{
		ShowControlBar(&m_wndToolBar,FALSE,FALSE);
		ShowControlBar(&m_wndToolBar2,TRUE,FALSE);
	}
	else
	{
		ShowControlBar(&m_wndToolBar2,FALSE,FALSE);
		ShowControlBar(&m_wndToolBar,TRUE,FALSE);
	}
}
