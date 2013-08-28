// DialogBarView.cpp : CDialogBarView 类的实现
//

#include "stdafx.h"
#include "DialogBar.h"

#include "DialogBarDoc.h"
#include "DialogBarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialogBarView

IMPLEMENT_DYNCREATE(CDialogBarView, CView)

BEGIN_MESSAGE_MAP(CDialogBarView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDialogBarView 构造/析构

CDialogBarView::CDialogBarView()
{
	// TODO: 在此处添加构造代码

}

CDialogBarView::~CDialogBarView()
{
}

BOOL CDialogBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDialogBarView 绘制

void CDialogBarView::OnDraw(CDC* /*pDC*/)
{
	CDialogBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDialogBarView 打印

BOOL CDialogBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDialogBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDialogBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CDialogBarView 诊断

#ifdef _DEBUG
void CDialogBarView::AssertValid() const
{
	CView::AssertValid();
}

void CDialogBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDialogBarDoc* CDialogBarView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDialogBarDoc)));
	return (CDialogBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CDialogBarView 消息处理程序
