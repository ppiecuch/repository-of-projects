// YonMultiwindowDlg.h : 头文件
//

#pragma once
#include "Dialog1.h"
#include "Dialog2.h"


// CYonMultiwindowDlg 对话框
class CYonMultiwindowDlg : public CDialog
{
// 构造
public:
	CYonMultiwindowDlg(CWnd* pParent = NULL);	// 标准构造函数

	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;

// 对话框数据
	enum { IDD = IDD_YONMULTIWINDOW_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	CDialog1 m_dlg1;
	CDialog2 m_dlg2;
	
};
