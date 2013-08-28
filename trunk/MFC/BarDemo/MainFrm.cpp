// MainFrm.cpp : CMainFrame ���ʵ��
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

	//����ControlBar 
	if (!m_wndMyBar1.Create(_T("�ҵĿ�����"),this,123)) 
	{ TRACE0("Failed to create mybar\n"); return -1; } 
	if (!m_wndMyBar2.Create(_T("�ҵĿ�����2"),this,CSize(100,100),TRUE,124))
	{ TRACE0("Failed to create mybar\n"); return -1; }


	//ͣ������
	m_wndMyBar1.SetBarStyle(m_wndMyBar1.GetBarStyle() | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC); 
	m_wndMyBar2.SetBarStyle(m_wndMyBar2.GetBarStyle() | CBRS_TOOLTIPS |
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMyBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMyBar2.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMyBar1, AFX_IDW_DOCKBAR_LEFT);///ͣ�������
	DockControlBar(&m_wndMyBar2, AFX_IDW_DOCKBAR_BOTTOM);///ͣ�����ұ�

	//�ڴ���ControlBar�󴴽��༭�ؼ�
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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




void CMainFrame::On32771()
{
	// TODO: �ڴ���������������
	TRACE("fdsafads");
}

void CMainFrame::OnUpdate32772(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(b);
}

void CMainFrame::On32772()
{
	// TODO: �ڴ���������������
	b=!b;

	//m_wndMyBar1.GetParentFrame()->ShowWindow(b);
	ShowControlBar(&m_wndMyBar1,b,false);
}
