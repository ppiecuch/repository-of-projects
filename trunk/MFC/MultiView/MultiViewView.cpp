// MultiViewView.cpp : CMultiViewView 类的实现
//

#include "stdafx.h"
#include "MultiView.h"

#include "MultiViewDoc.h"
#include "MultiViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiViewView

IMPLEMENT_DYNCREATE(CMultiViewView, CEditView)

BEGIN_MESSAGE_MAP(CMultiViewView, CEditView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMultiViewView 构造/析构

CMultiViewView::CMultiViewView()
{
	// TODO: 在此处添加构造代码

}

CMultiViewView::~CMultiViewView()
{
}

BOOL CMultiViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// 启用换行

	return bPreCreated;
}


// CMultiViewView 打印

BOOL CMultiViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认 CEditView 准备
	return CEditView::OnPreparePrinting(pInfo);
}

void CMultiViewView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 开始打印
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CMultiViewView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 结束打印
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CMultiViewView 诊断

#ifdef _DEBUG
void CMultiViewView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMultiViewView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMultiViewDoc* CMultiViewView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiViewDoc)));
	return (CMultiViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiViewView 消息处理程序
