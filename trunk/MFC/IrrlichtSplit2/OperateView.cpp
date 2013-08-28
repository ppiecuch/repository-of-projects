// OperateView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"
#include "OperateView.h"


// COperateView

IMPLEMENT_DYNCREATE(COperateView, CFormView)

COperateView::COperateView()
	: CFormView(COperateView::IDD)
{

}

COperateView::~COperateView()
{
}

void COperateView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COperateView, CFormView)
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

void COperateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	this->m_sheet.AddPage(&m_attributesPage);
	this->m_sheet.AddPage(&m_materialsPage);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	RECT rect;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	m_sheet.SetWindowPos(NULL, 0, 200, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}
