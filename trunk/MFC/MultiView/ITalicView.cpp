// ITalicView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MultiView.h"
#include "ITalicView.h"


// CITalicView

IMPLEMENT_DYNCREATE(CITalicView, CEditView)

CITalicView::CITalicView()
{

}

CITalicView::~CITalicView()
{
}

BEGIN_MESSAGE_MAP(CITalicView, CEditView)
END_MESSAGE_MAP()


// CITalicView ���

#ifdef _DEBUG
void CITalicView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CITalicView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CITalicView ��Ϣ�������

void CITalicView::OnDraw(CDC* pDC)
{
	CDocument* pDoc1 = GetDocument();
	CMultiViewDoc* pDoc = (CMultiViewDoc*)GetDocument();
	CFont fontItalic;
	//�������»��ߵ�б������
	fontItalic.CreateFontW(0,0,0,0,0,1,1,0,0,0,0,0,0,0);
	//����������
	CFont* pOldFont = pDC->SelectObject(&fontItalic);
	CRect rectClient;
	//�õ��ͻ����ڵĴ�С
	GetClientRect(rectClient);
	//��������ı�
	pDC->DrawText(pDoc->m_strText,rectClient,DT_WORDBREAK);
	//�ָ�ԭ��������
	pDC->SelectObject(pOldFont);
}
