// IrrlichtSplitDoc.cpp : CIrrlichtSplitDoc ���ʵ��
//

#include "stdafx.h"
#include "IrrlichtSplit.h"

#include "IrrlichtSplitDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIrrlichtSplitDoc

IMPLEMENT_DYNCREATE(CIrrlichtSplitDoc, CDocument)

BEGIN_MESSAGE_MAP(CIrrlichtSplitDoc, CDocument)
END_MESSAGE_MAP()


// CIrrlichtSplitDoc ����/����

CIrrlichtSplitDoc::CIrrlichtSplitDoc()
: m_nRenderMode(RENDER_MODE::PLANE)
, m_bUpdated(false)
{
	// TODO: �ڴ����һ���Թ������

}

CIrrlichtSplitDoc::~CIrrlichtSplitDoc()
{
}

BOOL CIrrlichtSplitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CIrrlichtSplitDoc ���л�

void CIrrlichtSplitDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CIrrlichtSplitDoc ���

#ifdef _DEBUG
void CIrrlichtSplitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIrrlichtSplitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIrrlichtSplitDoc ����
