// DialogBarView.cpp : CDialogBarView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDialogBarView ����/����

CDialogBarView::CDialogBarView()
{
	// TODO: �ڴ˴���ӹ������

}

CDialogBarView::~CDialogBarView()
{
}

BOOL CDialogBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDialogBarView ����

void CDialogBarView::OnDraw(CDC* /*pDC*/)
{
	CDialogBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CDialogBarView ��ӡ

BOOL CDialogBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDialogBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDialogBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CDialogBarView ���

#ifdef _DEBUG
void CDialogBarView::AssertValid() const
{
	CView::AssertValid();
}

void CDialogBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDialogBarDoc* CDialogBarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDialogBarDoc)));
	return (CDialogBarDoc*)m_pDocument;
}
#endif //_DEBUG


// CDialogBarView ��Ϣ�������
