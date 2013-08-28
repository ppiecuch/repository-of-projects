// Dialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "BarDemo.h"
#include "Dialog2.h"


// CDialog2 对话框

IMPLEMENT_DYNAMIC(CDialog2, CDialog)

CDialog2::CDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2::IDD, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialog)
END_MESSAGE_MAP()


// CDialog2 消息处理程序
