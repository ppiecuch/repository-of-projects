// BarDemoView.cpp : CBarDemoView 类的实现
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

// CBarDemoView 构造/析构

CBarDemoView::CBarDemoView()
	: CFormView(CBarDemoView::IDD)
{
	// TODO: 在此处添加构造代码

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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CBarDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CBarDemoView 诊断

#ifdef _DEBUG
void CBarDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBarDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBarDemoDoc* CBarDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBarDemoDoc)));
	return (CBarDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CBarDemoView 消息处理程序
