// BarDemoDoc.cpp : CBarDemoDoc ���ʵ��
//

#include "stdafx.h"
#include "BarDemo.h"

#include "BarDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBarDemoDoc

IMPLEMENT_DYNCREATE(CBarDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CBarDemoDoc, CDocument)
END_MESSAGE_MAP()


// CBarDemoDoc ����/����

CBarDemoDoc::CBarDemoDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBarDemoDoc::~CBarDemoDoc()
{
}

BOOL CBarDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CBarDemoDoc ���л�

void CBarDemoDoc::Serialize(CArchive& ar)
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


// CBarDemoDoc ���

#ifdef _DEBUG
void CBarDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBarDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBarDemoDoc ����
