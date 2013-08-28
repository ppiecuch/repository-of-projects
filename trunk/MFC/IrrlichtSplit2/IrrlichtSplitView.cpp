// IrrlichtSplitView.cpp : CIrrlichtSplitView ���ʵ��
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"

#include "IrrlichtSplitDoc.h"
#include "IrrlichtSplitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIrrlichtSplitView

IMPLEMENT_DYNCREATE(CIrrlichtSplitView, CFormView)

BEGIN_MESSAGE_MAP(CIrrlichtSplitView, CFormView)
END_MESSAGE_MAP()

// CIrrlichtSplitView ����/����

CIrrlichtSplitView::CIrrlichtSplitView()
	: CFormView(CIrrlichtSplitView::IDD)
{
	// TODO: �ڴ˴���ӹ������

}

CIrrlichtSplitView::~CIrrlichtSplitView()
{
}

void CIrrlichtSplitView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CIrrlichtSplitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CIrrlichtSplitView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CIrrlichtSplitView ���

#ifdef _DEBUG
void CIrrlichtSplitView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIrrlichtSplitView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIrrlichtSplitDoc* CIrrlichtSplitView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIrrlichtSplitDoc)));
	return (CIrrlichtSplitDoc*)m_pDocument;
}
#endif //_DEBUG


// CIrrlichtSplitView ��Ϣ�������
