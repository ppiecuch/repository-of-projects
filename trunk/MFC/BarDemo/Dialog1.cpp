// Dialog1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BarDemo.h"
#include "Dialog1.h"


// CDialog1 �Ի���

IMPLEMENT_DYNAMIC(CDialog1, CDialog)

CDialog1::CDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog1::IDD, pParent)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog1, CDialog)
END_MESSAGE_MAP()


// CDialog1 ��Ϣ�������
