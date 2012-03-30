// YonExampleMFCDoc.cpp : CYonExampleMFCDoc 类的实现
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


// CYonExampleMFCDoc 构造/析构

CYonExampleMFCDoc::CYonExampleMFCDoc()
{
	// TODO: 在此添加一次性构造代码

}

CYonExampleMFCDoc::~CYonExampleMFCDoc()
{
}

BOOL CYonExampleMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CYonExampleMFCDoc 序列化

void CYonExampleMFCDoc::Serialize(CArchive& ar)
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


// CYonExampleMFCDoc 诊断

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


// CYonExampleMFCDoc 命令
