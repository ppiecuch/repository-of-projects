#pragma once


// CAttributesPage �Ի���

class CAttributesPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAttributesPage)

public:
	CAttributesPage();
	virtual ~CAttributesPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_ATTRIBUTES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
