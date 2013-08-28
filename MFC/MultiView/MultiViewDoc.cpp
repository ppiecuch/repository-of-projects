// MultiViewDoc.cpp : CMultiViewDoc 类的实现
//

#include "stdafx.h"
#include "MultiView.h"

#include "MultiViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiViewDoc

IMPLEMENT_DYNCREATE(CMultiViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CMultiViewDoc, CDocument)
END_MESSAGE_MAP()


// CMultiViewDoc 构造/析构

CMultiViewDoc::CMultiViewDoc()
: m_strText(_T(""))
{
	// TODO: 在此添加一次性构造代码

}

CMultiViewDoc::~CMultiViewDoc()
{
}

BOOL CMultiViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMultiViewDoc 序列化

void CMultiViewDoc::Serialize(CArchive& ar)
{
	// CEditView 包含一个处理所有序列化的编辑控件
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
}


// CMultiViewDoc 诊断

#ifdef _DEBUG
void CMultiViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMultiViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMultiViewDoc 命令
