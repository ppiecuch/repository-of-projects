// BarDemoView.cpp : CBarDemoView ���ʵ��
//

#include "stdafx.h"
#include "BarDemo.h"

#include "BarDemoDoc.h"
#include "BarDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBarDemoView

IMPLEMENT_DYNCREATE(CBarDemoView, CFormView)

BEGIN_MESSAGE_MAP(CBarDemoView, CFormView)
END_MESSAGE_MAP()

// CBarDemoView ����/����

CBarDemoView::CBarDemoView()
	: CFormView(CBarDemoView::IDD)
{
	// TODO: �ڴ˴���ӹ������

}

CBarDemoView::~CBarDemoView()
{
}

void CBarDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CBarDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CBarDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CBarDemoView ���

#ifdef _DEBUG
void CBarDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBarDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBarDemoDoc* CBarDemoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBarDemoDoc)));
	return (CBarDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CBarDemoView ��Ϣ�������
