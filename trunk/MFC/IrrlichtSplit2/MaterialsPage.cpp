// MaterialsPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"
#include "MaterialsPage.h"


// CMaterialsPage �Ի���

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


// CMaterialsPage ��Ϣ�������
