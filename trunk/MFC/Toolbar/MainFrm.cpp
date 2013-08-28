// MainFrm.cpp : CMainFrame ���ʵ��
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
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
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

	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar(IDR_TOOLBAR2))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��ͣ������������ɾ��������
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



void CMainFrame::OnButton(UINT nID)
{
	m_bIndex=(int)(nID-ID_BUTTON_TEST1);
	swapToolbar();
}

void CMainFrame::OnUpdateButton(CCmdUI* pCmdUI)
{
	//��ť���¼��״̬���ú���
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
