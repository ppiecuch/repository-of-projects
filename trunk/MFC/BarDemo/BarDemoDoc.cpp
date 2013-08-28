// BarDemoDoc.cpp : CBarDemoDoc 类的实现
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


// CBarDemoDoc 构造/析构

CBarDemoDoc::CBarDemoDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBarDemoDoc::~CBarDemoDoc()
{
}

BOOL CBarDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CBarDemoDoc 序列化

void CBarDemoDoc::Serialize(CArchive& ar)
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


// CBarDemoDoc 诊断

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


// CBarDemoDoc 命令
