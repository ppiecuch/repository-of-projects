// IrrlichtSplitView.cpp : CIrrlichtSplitView 类的实现
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

// CIrrlichtSplitView 构造/析构

CIrrlichtSplitView::CIrrlichtSplitView()
	: CFormView(CIrrlichtSplitView::IDD)
{
	// TODO: 在此处添加构造代码

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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CIrrlichtSplitView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CIrrlichtSplitView 诊断

#ifdef _DEBUG
void CIrrlichtSplitView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIrrlichtSplitView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIrrlichtSplitDoc* CIrrlichtSplitView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIrrlichtSplitDoc)));
	return (CIrrlichtSplitDoc*)m_pDocument;
}
#endif //_DEBUG


// CIrrlichtSplitView 消息处理程序
