// DeliverDoc.cpp : CDeliverDoc ���ʵ��
//

#include "stdafx.h"
#include "Deliver.h"

#include "DeliverDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDeliverDoc

IMPLEMENT_DYNCREATE(CDeliverDoc, CDocument)

BEGIN_MESSAGE_MAP(CDeliverDoc, CDocument)
END_MESSAGE_MAP()


// CDeliverDoc ����/����

CDeliverDoc::CDeliverDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDeliverDoc::~CDeliverDoc()
{
}

BOOL CDeliverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDeliverDoc ���л�

void CDeliverDoc::Serialize(CArchive& ar)
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


// CDeliverDoc ���

#ifdef _DEBUG
void CDeliverDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDeliverDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDeliverDoc ����
