// MainFrm.cpp : CMainFrame ���ʵ��
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
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message)
	{
	case WM_DELIVER_DOWNLOAD:
		//TRACE("translate download msg\r\n");
		//��ѭ�������Ϸ����Լ�
		/*CWnd* pw=this->m_wndSplitter.GetPane(0,1);
		ASSERT(pw);

		::PostMessage(pw->m_hWnd,WM_DELIVER_DOWNLOAD,0,0);*/
		
		
		//�����������ղ���
		//CMainFrame::GetActiveView()->SendMessage(WM_DELIVER_DOWNLOAD);

		

		////��ѭ�������Ϸ����Լ�
		//m_wndSplitter.GetPane(0,1)->PostMessage(WM_DELIVER_DOWNLOAD);

		//���
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
