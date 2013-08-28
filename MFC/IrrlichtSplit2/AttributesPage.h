#pragma once


// CAttributesPage 对话框

class CAttributesPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAttributesPage)

public:
	CAttributesPage();
	virtual ~CAttributesPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ATTRIBUTES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
