// ITalicView.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiView.h"
#include "ITalicView.h"


// CITalicView

IMPLEMENT_DYNCREATE(CITalicView, CEditView)

CITalicView::CITalicView()
{

}

CITalicView::~CITalicView()
{
}

BEGIN_MESSAGE_MAP(CITalicView, CEditView)
END_MESSAGE_MAP()


// CITalicView 诊断

#ifdef _DEBUG
void CITalicView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CITalicView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CITalicView 消息处理程序

void CITalicView::OnDraw(CDC* pDC)
{
	CDocument* pDoc1 = GetDocument();
	CMultiViewDoc* pDoc = (CMultiViewDoc*)GetDocument();
	CFont fontItalic;
	//创建带下画线的斜体字体
	fontItalic.CreateFontW(0,0,0,0,0,1,1,0,0,0,0,0,0,0);
	//设置新字体
	CFont* pOldFont = pDC->SelectObject(&fontItalic);
	CRect rectClient;
	//得到客户窗口的大小
	GetClientRect(rectClient);
	//输出多行文本
	pDC->DrawText(pDoc->m_strText,rectClient,DT_WORDBREAK);
	//恢复原来的字体
	pDC->SelectObject(pOldFont);
}
