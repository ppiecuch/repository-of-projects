// RenderView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Deliver.h"
#include "RenderView.h"


// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CFormView)

CRenderView::CRenderView()
	: CFormView(CRenderView::IDD)
	, m_str(_T(""))
{

}

CRenderView::~CRenderView()
{
}

void CRenderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str);
}

BEGIN_MESSAGE_MAP(CRenderView, CFormView)
	ON_EN_CHANGE(IDC_EDIT1, &CRenderView::OnEnChangeEdit1)
	ON_MESSAGE(WM_DELIVER_UPLOAD,OnUpload)
END_MESSAGE_MAP()


// CRenderView ���

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRenderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRenderView ��Ϣ�������

void CRenderView::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	Context::GetInstance().str=m_str;
	CMainFrame *MF=(CMainFrame*)AfxGetMainWnd();
	MF->PostMessage(WM_DELIVER_DOWNLOAD);
	MF->PostMessage(WM_DELIVER_DOWNLOAD_TAB);
	//TRACE("PostMessage\r\n");
}

BOOL CRenderView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	///TRACE("receiver MSG\r\n");
	return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CRenderView::OnUpload(WPARAM wParam, LPARAM lParam)
{
	TRACE("Render upload\r\n");
	this->m_str=Context::GetInstance().str;
	UpdateData(false);
	return 0;
}
