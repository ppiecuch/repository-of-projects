// OperateView.cpp : 实现文件
//

#include "stdafx.h"
#include "IrrlichtSplit.h"
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
	ON_COMMAND_RANGE(IDC_RADIO_POINT,IDC_RADIO_PLANE,onRenderMode)
END_MESSAGE_MAP()


// COperateView 诊断

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


// COperateView 消息处理程序

void COperateView::onRenderMode(UINT nID)
{
	CButton *p_point=(CButton*)GetDlgItem(IDC_RADIO_POINT);
	CButton *p_line=(CButton*)GetDlgItem(IDC_RADIO_LINE);
	CButton *p_plane=(CButton*)GetDlgItem(IDC_RADIO_PLANE);
	CIrrlichtSplitDoc* pDoc=(CIrrlichtSplitDoc*) GetDocument();
	ASSERT_VALID(pDoc);
	if(p_point->GetCheck())
		pDoc->m_nRenderMode=CIrrlichtSplitDoc::POINT;
	else if(p_line->GetCheck())
		pDoc->m_nRenderMode=CIrrlichtSplitDoc::LINE;
	else
		pDoc->m_nRenderMode=CIrrlichtSplitDoc::PLANE;

	pDoc->m_bUpdated=true;

}

void COperateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CButton *p_plane=(CButton*)GetDlgItem(IDC_RADIO_PLANE);
	p_plane->SetCheck(1);
}
