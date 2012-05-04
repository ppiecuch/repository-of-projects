// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		tabs.Add(*m_pRenderView,_T("��Ⱦ��"),4);

		m_pTopView = (CTopView *)RUNTIME_CLASS( CTopView )->CreateObject();
		m_pTopView->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | LVS_SHOWSELALWAYS | LVS_REPORT,CRect(0,0,0,0), &m_MultiPaneCtrl, CTopView::IDD, NULL);
		tabs.Add(*m_pTopView,_T("����ͼ"),4);

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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_MultiPaneCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnDestroy()
{	
	m_MultiPaneCtrl.SaveState(AfxGetApp(),_T("GLLaborary"),_T("State"));
	CFrameWnd::OnDestroy();
	GLEnvironment::destroy();
}