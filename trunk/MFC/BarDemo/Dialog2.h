#pragma once
#include "resource.h"

// CDialog2 �Ի���

class CDialog2 : public CDialog
{
	DECLARE_DYNAMIC(CDialog2)

public:
	CDialog2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialog2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
