// ToolbarView.cpp : CToolbarView 类的实现
//

#include "stdafx.h"
#include "Toolbar.h"

#include "ToolbarDoc.h"
#include "ToolbarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolbarView

IMPLEMENT_DYNCREATE(CToolbarView, CView)

BEGIN_MESSAGE_MAP(CToolbarView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolbarView 构造/析构

CToolbarView::CToolbarView()
{
	// TODO: 在此处添加构造代码

}

CToolbarView::~CToolbarView()
{
}

BOOL CToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CToolbarView 绘制

void CToolbarView::OnDraw(CDC* /*pDC*/)
{
	CToolbarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CToolbarView 打印

BOOL CToolbarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CToolbarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CToolbarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CToolbarView 诊断

#ifdef _DEBUG
void CToolbarView::AssertValid() const
{
	CView::AssertValid();
}

void CToolbarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolbarDoc* CToolbarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolbarDoc)));
	return (CToolbarDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolbarView 消息处理程序
