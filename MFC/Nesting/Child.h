#pragma once


// CChild 对话框

class CChild : public CDialog
{
	DECLARE_DYNAMIC(CChild)

public:
	CChild(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChild();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
