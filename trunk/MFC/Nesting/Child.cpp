// Child.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Nesting.h"
#include "Child.h"


// CChild �Ի���

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


// CChild ��Ϣ�������
