// ToolbarDoc.cpp : CToolbarDoc ���ʵ��
//

#include "stdafx.h"
#include "Toolbar.h"

#include "ToolbarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolbarDoc

IMPLEMENT_DYNCREATE(CToolbarDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolbarDoc, CDocument)
END_MESSAGE_MAP()


// CToolbarDoc ����/����

CToolbarDoc::CToolbarDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CToolbarDoc::~CToolbarDoc()
{
}

BOOL CToolbarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CToolbarDoc ���л�

void CToolbarDoc::Serialize(CArchive& ar)
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


// CToolbarDoc ���

#ifdef _DEBUG
void CToolbarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolbarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CToolbarDoc ����
