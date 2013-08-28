// OperateView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Deliver.h"
#include "OperateView.h"


// COperateView

IMPLEMENT_DYNCREATE(COperateView, CFormView)

COperateView::COperateView()
	: CFormView(COperateView::IDD)
	, m_str(_T(""))
{
}

COperateView::~COperateView()
{
}

void COperateView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(COperateView, CFormView)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DELIVER_DOWNLOAD,OnDownload)
	ON_EN_CHANGE(IDC_EDIT1, &COperateView::OnEnChangeEdit1)
END_MESSAGE_MAP()


// COperateView ���

#ifdef _DEBUG
void COperateView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COperateView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COperateView ��Ϣ�������

void COperateView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
}

LRESULT COperateView::OnDownload(WPARAM wParam, LPARAM lParam)
{
	TRACE("Operate download\r\n");
	
	this->m_str=Context::GetInstance().str;
	UpdateData(false);
	return 0;
}

BOOL COperateView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL COperateView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	switch(pMsg->message)
	{
	case WM_DELIVER_DOWNLOAD_TAB:
		m_dialog1.SendMessage(WM_DELIVER_DOWNLOAD_TAB);
		return true;
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void COperateView::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	UpdateData(true);
	Context::GetInstance().str=m_str;
	CMainFrame *MF=(CMainFrame*)AfxGetMainWnd();
	MF->PostMessage(WM_DELIVER_UPLOAD);
}

void COperateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_tab.InsertItem(0,TEXT("���"));
	//����һ
	m_dialog1.Create(IDD_DIALOG1,GetDlgItem(IDC_TAB1));
	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.top+=22;
	rect.bottom-=2;
	rect.left+=2;
	rect.right-=3;
	m_dialog1.MoveWindow(&rect);
	m_dialog1.ShowWindow(SW_SHOW);

}
