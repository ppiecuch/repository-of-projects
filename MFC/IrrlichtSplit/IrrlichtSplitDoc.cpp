// IrrlichtSplitDoc.cpp : CIrrlichtSplitDoc 类的实现
//

#include "stdafx.h"
#include "IrrlichtSplit.h"

#include "IrrlichtSplitDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIrrlichtSplitDoc

IMPLEMENT_DYNCREATE(CIrrlichtSplitDoc, CDocument)

BEGIN_MESSAGE_MAP(CIrrlichtSplitDoc, CDocument)
END_MESSAGE_MAP()


// CIrrlichtSplitDoc 构造/析构

CIrrlichtSplitDoc::CIrrlichtSplitDoc()
: m_nRenderMode(RENDER_MODE::PLANE)
, m_bUpdated(false)
{
	// TODO: 在此添加一次性构造代码

}

CIrrlichtSplitDoc::~CIrrlichtSplitDoc()
{
}

BOOL CIrrlichtSplitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CIrrlichtSplitDoc 序列化

void CIrrlichtSplitDoc::Serialize(CArchive& ar)
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


// CIrrlichtSplitDoc 诊断

#ifdef _DEBUG
void CIrrlichtSplitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIrrlichtSplitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIrrlichtSplitDoc 命令
