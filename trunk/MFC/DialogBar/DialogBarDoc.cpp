// DialogBarDoc.cpp : CDialogBarDoc ���ʵ��
//

#include "stdafx.h"
#include "DialogBar.h"

#include "DialogBarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialogBarDoc

IMPLEMENT_DYNCREATE(CDialogBarDoc, CDocument)

BEGIN_MESSAGE_MAP(CDialogBarDoc, CDocument)
END_MESSAGE_MAP()


// CDialogBarDoc ����/����

CDialogBarDoc::CDialogBarDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDialogBarDoc::~CDialogBarDoc()
{
}

BOOL CDialogBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDialogBarDoc ���л�

void CDialogBarDoc::Serialize(CArchive& ar)
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


// CDialogBarDoc ���

#ifdef _DEBUG
void CDialogBarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDialogBarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDialogBarDoc ����
