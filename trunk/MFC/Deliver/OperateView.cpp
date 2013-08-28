// OperateView.cpp : 实现文件
//

#include "stdafx.h"
#include "Deliver.h"
#include "OperateView.h"


// COperateView

IMPLEMENT_DYNCREATE(COperateView, CFormView)

COperateView::COperateView()
	: CFormView(COperateView::IDD)
	, m_str(_T(""))
{
}

COperateView::~COperateView()
{
}

void COperateView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(COperateView, CFormView)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DELIVER_DOWNLOAD,OnDownload)
	ON_EN_CHANGE(IDC_EDIT1, &COperateView::OnEnChangeEdit1)
END_MESSAGE_MAP()


// COperateView 诊断

#ifdef _DEBUG
void COperateView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COperateView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COperateView 消息处理程序

void COperateView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
}

LRESULT COperateView::OnDownload(WPARAM wParam, LPARAM lParam)
{
	TRACE("Operate download\r\n");
	
	this->m_str=Context::GetInstance().str;
	UpdateData(false);
	return 0;
}

BOOL COperateView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL COperateView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch(pMsg->message)
	{
	case WM_DELIVER_DOWNLOAD_TAB:
		m_dialog1.SendMessage(WM_DELIVER_DOWNLOAD_TAB);
		return true;
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void COperateView::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(true);
	Context::GetInstance().str=m_str;
	CMainFrame *MF=(CMainFrame*)AfxGetMainWnd();
	MF->PostMessage(WM_DELIVER_UPLOAD);
}

void COperateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_tab.InsertItem(0,TEXT("简介"));
	//方法一
	m_dialog1.Create(IDD_DIALOG1,GetDlgItem(IDC_TAB1));
	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.top+=22;
	rect.bottom-=2;
	rect.left+=2;
	rect.right-=3;
	m_dialog1.MoveWindow(&rect);
	m_dialog1.ShowWindow(SW_SHOW);

}
