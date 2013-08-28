// DialogBarDoc.cpp : CDialogBarDoc 类的实现
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


// CDialogBarDoc 构造/析构

CDialogBarDoc::CDialogBarDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDialogBarDoc::~CDialogBarDoc()
{
}

BOOL CDialogBarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDialogBarDoc 序列化

void CDialogBarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CDialogBarDoc 诊断

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


// CDialogBarDoc 命令
