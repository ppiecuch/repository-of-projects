// TabDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Deliver.h"
#include "TabDialog.h"


// CTabDialog 对话框

IMPLEMENT_DYNAMIC(CTabDialog, CDialog)

CTabDialog::CTabDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTabDialog::IDD, pParent)
	, m_str(_T(""))
{

}

CTabDialog::~CTabDialog()
{
}

void CTabDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str);
}


BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
	ON_MESSAGE(WM_DELIVER_DOWNLOAD_TAB,OnDownload)
END_MESSAGE_MAP()


// CTabDialog 消息处理程序

LRESULT CTabDialog::OnDownload(WPARAM wParam, LPARAM lParam)
{
	TRACE("Tab download\r\n");
	
	this->m_str=Context::GetInstance().str;
	UpdateData(false);
	return 0;
}