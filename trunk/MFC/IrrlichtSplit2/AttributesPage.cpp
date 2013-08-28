// AttributesPage.cpp : 实现文件
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"
#include "AttributesPage.h"


// CAttributesPage 对话框

IMPLEMENT_DYNAMIC(CAttributesPage, CPropertyPage)

CAttributesPage::CAttributesPage()
	: CPropertyPage(CAttributesPage::IDD)
{

}

CAttributesPage::~CAttributesPage()
{
}

void CAttributesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAttributesPage, CPropertyPage)
END_MESSAGE_MAP()


// CAttributesPage 消息处理程序
