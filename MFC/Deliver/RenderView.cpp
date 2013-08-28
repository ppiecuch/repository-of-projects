// RenderView.cpp : 实现文件
//

#include "stdafx.h"
#include "Deliver.h"
#include "RenderView.h"


// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CFormView)

CRenderView::CRenderView()
	: CFormView(CRenderView::IDD)
	, m_str(_T(""))
{

}

CRenderView::~CRenderView()
{
}

void CRenderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str);
}

BEGIN_MESSAGE_MAP(CRenderView, CFormView)
	ON_EN_CHANGE(IDC_EDIT1, &CRenderView::OnEnChangeEdit1)
	ON_MESSAGE(WM_DELIVER_UPLOAD,OnUpload)
END_MESSAGE_MAP()


// CRenderView 诊断

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRenderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRenderView 消息处理程序

void CRenderView::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	Context::GetInstance().str=m_str;
	CMainFrame *MF=(CMainFrame*)AfxGetMainWnd();
	MF->PostMessage(WM_DELIVER_DOWNLOAD);
	MF->PostMessage(WM_DELIVER_DOWNLOAD_TAB);
	//TRACE("PostMessage\r\n");
}

BOOL CRenderView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	///TRACE("receiver MSG\r\n");
	return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CRenderView::OnUpload(WPARAM wParam, LPARAM lParam)
{
	TRACE("Render upload\r\n");
	this->m_str=Context::GetInstance().str;
	UpdateData(false);
	return 0;
}
