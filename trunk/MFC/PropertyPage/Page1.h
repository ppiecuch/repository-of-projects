#pragma once


// CPage1 �Ի���

class CPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1();
	virtual ~CPage1();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_SMALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
