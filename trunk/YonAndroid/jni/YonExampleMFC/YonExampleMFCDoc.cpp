// YonExampleMFCDoc.cpp : CYonExampleMFCDoc ���ʵ��
//

#include "stdafx.h"
#include "YonExampleMFC.h"

#include "YonExampleMFCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYonExampleMFCDoc

IMPLEMENT_DYNCREATE(CYonExampleMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CYonExampleMFCDoc, CDocument)
END_MESSAGE_MAP()


// CYonExampleMFCDoc ����/����

CYonExampleMFCDoc::CYonExampleMFCDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CYonExampleMFCDoc::~CYonExampleMFCDoc()
{
}

BOOL CYonExampleMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CYonExampleMFCDoc ���л�

void CYonExampleMFCDoc::Serialize(CArchive& ar)
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


// CYonExampleMFCDoc ���

#ifdef _DEBUG
void CYonExampleMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYonExampleMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CYonExampleMFCDoc ����
