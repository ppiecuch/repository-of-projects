// MaterialsPage.cpp : 实现文件
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"
#include "MaterialsPage.h"


// CMaterialsPage 对话框

IMPLEMENT_DYNAMIC(CMaterialsPage, CPropertyPage)

CMaterialsPage::CMaterialsPage()
	: CPropertyPage(CMaterialsPage::IDD)
{

}

CMaterialsPage::~CMaterialsPage()
{
}

void CMaterialsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMaterialsPage, CPropertyPage)
END_MESSAGE_MAP()


// CMaterialsPage 消息处理程序
