// ColorSelectorDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CColorSelectorDlg 对话框
class CColorSelectorDlg : public CDialog
{
// 构造
public:
	CColorSelectorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLORSELECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_e;
	afx_msg void OnEnSetfocusEdit3();
	CButton m_b;
	afx_msg void OnClickStatic();
	CStatic m_s;
};
