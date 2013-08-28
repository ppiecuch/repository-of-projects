// 01ImageListDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include <vector>


// CMy01ImageListDlg 对话框
class CMy01ImageListDlg : public CDialog
{
// 构造
public:
	CMy01ImageListDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY01IMAGELIST_DIALOG };

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
	void enumFiles(const wchar_t* szSourceDir);
public:
	CListCtrl m_ListCtrl;
	CImageList m_ImageList;
	std::vector<CString> m_images;


	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
