// MultiViewDoc.cpp : CMultiViewDoc ���ʵ��
//

#include "stdafx.h"
#include "MultiView.h"

#include "MultiViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiViewDoc

IMPLEMENT_DYNCREATE(CMultiViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CMultiViewDoc, CDocument)
END_MESSAGE_MAP()


// CMultiViewDoc ����/����

CMultiViewDoc::CMultiViewDoc()
: m_strText(_T(""))
{
	// TODO: �ڴ����һ���Թ������

}

CMultiViewDoc::~CMultiViewDoc()
{
}

BOOL CMultiViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMultiViewDoc ���л�

void CMultiViewDoc::Serialize(CArchive& ar)
{
	// CEditView ����һ�������������л��ı༭�ؼ�
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
}


// CMultiViewDoc ���

#ifdef _DEBUG
void CMultiViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMultiViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMultiViewDoc ����
