// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Deliver.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_DELIVER_UPLOAD,OnUpload)
	//ON_MESSAGE(WM_DELIVER_DOWNLOAD,OnDownload)
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

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this,1, 2);
	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CRenderView),CSize(700,0),pContext)||
	!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(COperateView),CSize(0,0),pContext))
	{
		m_wndSplitter.DestroyWindow();
		return false;
	}
	return true;

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




/*LRESULT CMainFrame::OnDownload(WPARAM wParam, LPARAM lParam)
{
	TRACE("download\r\n");
	
	
	return 0;
}*/
LRESULT CMainFrame::OnUpload(WPARAM wParam, LPARAM lParam)
{
	TRACE("upload\r\n");
	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(pMsg->message)
	{
	case WM_DELIVER_DOWNLOAD:
		//TRACE("translate download msg\r\n");
		//死循环，不断发给自己
		/*CWnd* pw=this->m_wndSplitter.GetPane(0,1);
		ASSERT(pw);

		::PostMessage(pw->m_hWnd,WM_DELIVER_DOWNLOAD,0,0);*/
		
		
		//不卡，但接收不到
		//CMainFrame::GetActiveView()->SendMessage(WM_DELIVER_DOWNLOAD);

		

		////死循环，不断发给自己
		//m_wndSplitter.GetPane(0,1)->PostMessage(WM_DELIVER_DOWNLOAD);

		//解决
		m_wndSplitter.GetPane(0,1)->SendMessage(WM_DELIVER_DOWNLOAD);
		
		return true;
	case WM_DELIVER_UPLOAD:
		m_wndSplitter.GetPane(0,0)->SendMessage(WM_DELIVER_UPLOAD);
		return true;
	case WM_DELIVER_DOWNLOAD_TAB:
		m_wndSplitter.GetPane(0,1)->PostMessage(WM_DELIVER_DOWNLOAD_TAB);
		return true;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}
