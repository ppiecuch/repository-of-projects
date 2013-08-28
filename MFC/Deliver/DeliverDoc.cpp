// DeliverDoc.cpp : CDeliverDoc 类的实现
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


// CDeliverDoc 构造/析构

CDeliverDoc::CDeliverDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDeliverDoc::~CDeliverDoc()
{
}

BOOL CDeliverDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDeliverDoc 序列化

void CDeliverDoc::Serialize(CArchive& ar)
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


// CDeliverDoc 诊断

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


// CDeliverDoc 命令
