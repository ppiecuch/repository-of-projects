// YonDoublewindowDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "RenderStatic.h"


// CYonDoublewindowDlg 对话框
class CYonDoublewindowDlg : public CDialog
{
// 构造
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;

public:
	CYonDoublewindowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YONDOUBLEWINDOW_DIALOG };

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
	//CStatic m_static1;
	//CStatic m_static2;
	CRenderStatic m_static1;
	CRenderStatic m_static2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
