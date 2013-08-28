#pragma once


// CMaterialsPage 对话框

class CMaterialsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMaterialsPage)

public:
	CMaterialsPage();
	virtual ~CMaterialsPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MATERIALS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
