// YonExampleSDIDoc.cpp : CYonExampleSDIDoc 类的实现
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


// CYonExampleSDIDoc 构造/析构

CYonExampleSDIDoc::CYonExampleSDIDoc()
{
	// TODO: 在此添加一次性构造代码

}

CYonExampleSDIDoc::~CYonExampleSDIDoc()
{
}

BOOL CYonExampleSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CYonExampleSDIDoc 序列化

void CYonExampleSDIDoc::Serialize(CArchive& ar)
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


// CYonExampleSDIDoc 诊断

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


// CYonExampleSDIDoc 命令
