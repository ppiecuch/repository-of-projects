// FloatTestDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_FLOATTESTDLG_H__74F00C28_A0D6_11D2_898F_8050F6598430__INCLUDED_)
#define AFX_FLOATTESTDLG_H__74F00C28_A0D6_11D2_898F_8050F6598430__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFloatTestDlg dialog

class CFloatTestDlg : public CDialog
{
// Construction
public:
	CFloatTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFloatTestDlg)
	enum { IDD = IDD_FLOATTEST_DIALOG };
	CButton	m_CheckCommas;
	CEdit	m_Decimals;
	CFloatEdit	m_FloatEdit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	afx_msg LRESULT  onEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  onEnter2(WPARAM wParam, LPARAM lParam);

	const static UINT WM_ENTER,WM_ENTER2;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFloatTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonNegative();
	afx_msg void OnButtonPositive();
	afx_msg void OnCheckCommas();
	afx_msg void OnKillfocusEditDecimals();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CFloatEdit m_FloatEdit2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATTESTDLG_H__74F00C28_A0D6_11D2_898F_8050F6598430__INCLUDED_)
