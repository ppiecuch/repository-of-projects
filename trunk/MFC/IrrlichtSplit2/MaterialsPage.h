#pragma once


// CMaterialsPage �Ի���

class CMaterialsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMaterialsPage)

public:
	CMaterialsPage();
	virtual ~CMaterialsPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_MATERIALS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
