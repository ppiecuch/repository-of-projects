// SDIFT2Doc.cpp : CSDIFT2Doc 类的实现
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


// CSDIFT2Doc 构造/析构

CSDIFT2Doc::CSDIFT2Doc()
{
	// TODO: 在此添加一次性构造代码

}

CSDIFT2Doc::~CSDIFT2Doc()
{
}

BOOL CSDIFT2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSDIFT2Doc 序列化

void CSDIFT2Doc::Serialize(CArchive& ar)
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


// CSDIFT2Doc 诊断

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


// CSDIFT2Doc 命令
