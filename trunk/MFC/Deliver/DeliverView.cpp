// DeliverView.cpp : CDeliverView ���ʵ��
//

#include "stdafx.h"
#include "Deliver.h"

#include "DeliverDoc.h"
#include "DeliverView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeliverView

IMPLEMENT_DYNCREATE(CDeliverView, CFormView)

BEGIN_MESSAGE_MAP(CDeliverView, CFormView)
END_MESSAGE_MAP()

// CDeliverView ����/����

CDeliverView::CDeliverView()
	: CFormView(CDeliverView::IDD)
{
	// TODO: �ڴ˴���ӹ������

}

CDeliverView::~CDeliverView()
{
}

void CDeliverView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CDeliverView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CDeliverView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CDeliverView ���

#ifdef _DEBUG
void CDeliverView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeliverView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeliverDoc* CDeliverView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeliverDoc)));
	return (CDeliverDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeliverView ��Ϣ�������
