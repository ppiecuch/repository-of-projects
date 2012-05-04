// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "GLLaborary.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HGLRC	g_pRenderRC = NULL;
CDC *	g_pRenderDC = NULL;

HGLRC	g_pTopRC = NULL;
CDC *	g_pTopDC = NULL;
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
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

	

	// Creation of MultiPaneCtrl object.
	if(m_MultiPaneCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),AFX_IDW_PANE_FIRST)==false) return -1;

	CImageList imagelist,imagelistDis, imagelistSys;
	CBitmap bmp,bmpDis, bmpSys;

	imagelist.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmp.LoadBitmap(IDB_BITMAP_TAB_DISABLE);
	imagelist.Add(&bmp,RGB(255,0,255));

	imagelistDis.Create(16,16,ILC_COLOR24 | ILC_MASK,7,0);
	bmpDis.LoadBitmap(IDB_BITMAP_SYS);
	imagelistDis.Add(&bmpDis,RGB(255,0,255));

	imagelistSys.Create(14,14,ILC_COLOR24 | ILC_MASK,7,0);
	bmpSys.LoadBitmap(IDB_BITMAP_TAB_ENABLE);
	imagelistSys.Add(&bmpSys,RGB(255,0,255));

	m_MultiPaneCtrl.SetImageLists(&imagelist,&imagelistDis);
	m_MultiPaneCtrl.SetSystemImageList(&imagelistSys);

	m_MultiPaneCtrl.SetCursors(IDC_CURSOR_TAB,IDC_CURSOR_SPLIT_HORZ,IDC_CURSOR_SPLIT_VERT,IDC_CURSOR_OUT_ENABLE,IDC_CURSOR_OUT_DISABLE);

	m_MultiPaneCtrl.SetDockMarkers(MarkersLayoutC(),DockingMarkers::Params(40,true,14),50);
	m_MultiPaneCtrl.RemoveTabEnable(true);
	m_MultiPaneCtrl.DragTabEnable(true);

	// Loading state or creation default state.
	try
	{

		m_pRenderView = (CRenderView *)RUNTIME_CLASS( CRenderView )->CreateObject();
		m_pRenderView->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0), &m_MultiPaneCtrl, CRenderView::IDD, NULL);
		tabs.Add(*m_pRenderView,_T("渲染区"),4);

		m_pTopView = (CTopView *)RUNTIME_CLASS( CTopView )->CreateObject();
		m_pTopView->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0), &m_MultiPaneCtrl, CTopView::IDD, NULL);
		tabs.Add(*m_pTopView,_T("顶视图"),4);

		if(m_MultiPaneCtrl.LoadState(AfxGetApp(),_T("GLLaborary"), _T("State"),&tabs,false)==false)
		{

			HPANE hRender = m_MultiPaneCtrl.ConvertToLine(NULL,true);
			m_MultiPaneCtrl.AddTab(hRender,tabs[0]);

			HPANE hTop = m_MultiPaneCtrl.Add(NULL);
			m_MultiPaneCtrl.AddTab(hTop,tabs[1]);

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
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_MultiPaneCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnDestroy()
{	
	m_MultiPaneCtrl.SaveState(AfxGetApp(),_T("GLLaborary"),_T("State"));
	CFrameWnd::OnDestroy();
	GLEnvironment::destroy();
}