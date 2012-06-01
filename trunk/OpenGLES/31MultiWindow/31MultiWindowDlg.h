// 31MultiWindowDlg.h : 头文件
//

#pragma once

#include "Dialog1.h"
#include "Dialog2.h"


// CMy31MultiWindowDlg 对话框
class CMy31MultiWindowDlg : public CDialog
{
// 构造
public:
	CMy31MultiWindowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY31MULTIWINDOW_DIALOG };

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

	CDialog1 m_dlg1;
	CDialog2 m_dlg2;
};
