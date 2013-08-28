// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MultiView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_ITALIC, &CMainFrame::OnWindowItalic)
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
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
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

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




void CMainFrame::OnWindowItalic()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();//获得子窗口
	//获得当前活动窗口
	CMultiViewDoc* pDocument = (CMultiViewDoc*)pActiveChild->GetActiveDocument();
	CString strText;
	CEditView* pView = (CEditView*)pActiveChild->GetActiveView();//获得当前视图
	//获得视图中编辑控件的文本内容
	pView->GetEditCtrl().GetWindowText(strText);
	TRACE(strText);
	if(strText!="")
		pDocument->m_strText = strText;
	//获得新的文档模板的指针
	CDocTemplate* pTemplate = ((CMultiViewApp*)AfxGetApp())->m_pTemplateItalic;
	//Use to test your assumptions about the validity of an object’s internal state. 
	//ASSERT_VALID calls the AssertValid member function of the object passed as its argument. 
	//In the Release version of MFC, ASSERT_VALID does nothing. In the Debug version, 
	//it validates the pointer, checks against NULL, and calls the object’s own AssertValid member functions. 
	//If any of these tests fails, this displays an alert message in the same manner as ASSERT.
	ASSERT_VALID(pTemplate);
	//创建新的框架窗口
	CFrameWnd* pFrame=pTemplate->CreateNewFrame(pDocument,pActiveChild);
	pTemplate->InitialUpdateFrame(pFrame,pDocument);
}
