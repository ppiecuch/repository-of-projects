// YonExampleSDIDoc.cpp : CYonExampleSDIDoc ���ʵ��
//

#include "stdafx.h"
#include "YonExampleSDI.h"

#include "YonExampleSDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYonExampleSDIDoc

IMPLEMENT_DYNCREATE(CYonExampleSDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CYonExampleSDIDoc, CDocument)
END_MESSAGE_MAP()


// CYonExampleSDIDoc ����/����

CYonExampleSDIDoc::CYonExampleSDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CYonExampleSDIDoc::~CYonExampleSDIDoc()
{
}

BOOL CYonExampleSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CYonExampleSDIDoc ���л�

void CYonExampleSDIDoc::Serialize(CArchive& ar)
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


// CYonExampleSDIDoc ���

#ifdef _DEBUG
void CYonExampleSDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYonExampleSDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CYonExampleSDIDoc ����
