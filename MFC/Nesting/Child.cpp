// Child.cpp : 实现文件
//

#include "stdafx.h"
#include "Nesting.h"
#include "Child.h"


// CChild 对话框

IMPLEMENT_DYNAMIC(CChild, CDialog)

CChild::CChild(CWnd* pParent /*=NULL*/)
	: CDialog(CChild::IDD, pParent)
{

}

CChild::~CChild()
{
}

void CChild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChild, CDialog)
END_MESSAGE_MAP()


// CChild 消息处理程序
