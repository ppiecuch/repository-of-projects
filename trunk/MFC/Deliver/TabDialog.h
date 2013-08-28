#pragma once
#include "Context.h"


// CTabDialog 对话框

class CTabDialog : public CDialog
{
	DECLARE_DYNAMIC(CTabDialog)

public:
	CTabDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_str;
	afx_msg LRESULT OnDownload(WPARAM wParam, LPARAM lParam);
};
