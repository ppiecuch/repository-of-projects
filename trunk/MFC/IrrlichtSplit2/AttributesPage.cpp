// AttributesPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IrrlichtSplit2.h"
#include "AttributesPage.h"


// CAttributesPage �Ի���

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


// CAttributesPage ��Ϣ�������
