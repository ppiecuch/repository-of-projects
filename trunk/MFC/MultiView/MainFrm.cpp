// MainFrm.cpp : CMainFrame ���ʵ��
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
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
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

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������




void CMainFrame::OnWindowItalic()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();//����Ӵ���
	//��õ�ǰ�����
	CMultiViewDoc* pDocument = (CMultiViewDoc*)pActiveChild->GetActiveDocument();
	CString strText;
	CEditView* pView = (CEditView*)pActiveChild->GetActiveView();//��õ�ǰ��ͼ
	//�����ͼ�б༭�ؼ����ı�����
	pView->GetEditCtrl().GetWindowText(strText);
	TRACE(strText);
	if(strText!="")
		pDocument->m_strText = strText;
	//����µ��ĵ�ģ���ָ��
	CDocTemplate* pTemplate = ((CMultiViewApp*)AfxGetApp())->m_pTemplateItalic;
	//Use to test your assumptions about the validity of an object��s internal state. 
	//ASSERT_VALID calls the AssertValid member function of the object passed as its argument. 
	//In the Release version of MFC, ASSERT_VALID does nothing. In the Debug version, 
	//it validates the pointer, checks against NULL, and calls the object��s own AssertValid member functions. 
	//If any of these tests fails, this displays an alert message in the same manner as ASSERT.
	ASSERT_VALID(pTemplate);
	//�����µĿ�ܴ���
	CFrameWnd* pFrame=pTemplate->CreateNewFrame(pDocument,pActiveChild);
	pTemplate->InitialUpdateFrame(pFrame,pDocument);
}
