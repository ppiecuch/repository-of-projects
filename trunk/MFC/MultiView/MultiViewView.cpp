// MultiViewView.cpp : CMultiViewView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMultiViewView ����/����

CMultiViewView::CMultiViewView()
{
	// TODO: �ڴ˴���ӹ������

}

CMultiViewView::~CMultiViewView()
{
}

BOOL CMultiViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// ���û���

	return bPreCreated;
}


// CMultiViewView ��ӡ

BOOL CMultiViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ�� CEditView ׼��
	return CEditView::OnPreparePrinting(pInfo);
}

void CMultiViewView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Ĭ�� CEditView ��ʼ��ӡ
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CMultiViewView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Ĭ�� CEditView ������ӡ
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CMultiViewView ���

#ifdef _DEBUG
void CMultiViewView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMultiViewView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMultiViewDoc* CMultiViewView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiViewDoc)));
	return (CMultiViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiViewView ��Ϣ�������
