// MyDialogBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogBar.h"
#include "MyDialogBar.h"


// CMyDialogBar 对话框

IMPLEMENT_DYNAMIC(CMyDialogBar, CDialog)

CMyDialogBar::CMyDialogBar(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDialogBar::IDD, pParent)
{

}

CMyDialogBar::~CMyDialogBar()
{
}

void CMyDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDialogBar, CDialog)
END_MESSAGE_MAP()


// CMyDialogBar 消息处理程序
