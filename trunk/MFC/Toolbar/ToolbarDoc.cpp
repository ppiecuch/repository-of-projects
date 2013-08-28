// ToolbarDoc.cpp : CToolbarDoc 类的实现
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


// CToolbarDoc 构造/析构

CToolbarDoc::CToolbarDoc()
{
	// TODO: 在此添加一次性构造代码

}

CToolbarDoc::~CToolbarDoc()
{
}

BOOL CToolbarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CToolbarDoc 序列化

void CToolbarDoc::Serialize(CArchive& ar)
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


// CToolbarDoc 诊断

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


// CToolbarDoc 命令
