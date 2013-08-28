// ToolbarView.cpp : CToolbarView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolbarView ����/����

CToolbarView::CToolbarView()
{
	// TODO: �ڴ˴���ӹ������

}

CToolbarView::~CToolbarView()
{
}

BOOL CToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CToolbarView ����

void CToolbarView::OnDraw(CDC* /*pDC*/)
{
	CToolbarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CToolbarView ��ӡ

BOOL CToolbarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CToolbarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CToolbarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CToolbarView ���

#ifdef _DEBUG
void CToolbarView::AssertValid() const
{
	CView::AssertValid();
}

void CToolbarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolbarDoc* CToolbarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolbarDoc)));
	return (CToolbarDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolbarView ��Ϣ�������
