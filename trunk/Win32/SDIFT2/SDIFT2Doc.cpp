// SDIFT2Doc.cpp : CSDIFT2Doc ���ʵ��
//

#include "stdafx.h"
#include "SDIFT2.h"

#include "SDIFT2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIFT2Doc

IMPLEMENT_DYNCREATE(CSDIFT2Doc, CDocument)

BEGIN_MESSAGE_MAP(CSDIFT2Doc, CDocument)
END_MESSAGE_MAP()


// CSDIFT2Doc ����/����

CSDIFT2Doc::CSDIFT2Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CSDIFT2Doc::~CSDIFT2Doc()
{
}

BOOL CSDIFT2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSDIFT2Doc ���л�

void CSDIFT2Doc::Serialize(CArchive& ar)
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


// CSDIFT2Doc ���

#ifdef _DEBUG
void CSDIFT2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIFT2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSDIFT2Doc ����
