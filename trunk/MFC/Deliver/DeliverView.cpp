// DeliverView.cpp : CDeliverView 类的实现
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

// CDeliverView 构造/析构

CDeliverView::CDeliverView()
	: CFormView(CDeliverView::IDD)
{
	// TODO: 在此处添加构造代码

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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CDeliverView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CDeliverView 诊断

#ifdef _DEBUG
void CDeliverView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeliverView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDeliverDoc* CDeliverView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeliverDoc)));
	return (CDeliverDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeliverView 消息处理程序
