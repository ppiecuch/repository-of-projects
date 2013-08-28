#pragma once


// CMyDialogBar 对话框

class CMyDialogBar : public CDialog
{
	DECLARE_DYNAMIC(CMyDialogBar)

public:
	CMyDialogBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDialogBar();

// 对话框数据
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
