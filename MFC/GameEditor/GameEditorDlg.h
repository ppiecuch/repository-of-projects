// GameEditorDlg.h : header file
//

#if !defined(AFX_GAMEEDITORDLG_H__676710E1_8AAC_47E0_BED5_1F17B7D936BC__INCLUDED_)
#define AFX_GAMEEDITORDLG_H__676710E1_8AAC_47E0_BED5_1F17B7D936BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "editor.h"

/////////////////////////////////////////////////////////////////////////////
// CGameEditorDlg dialog

class CGameEditorDlg : public CDialog
{
// Construction
public:
	CGameEditorDlg(CWnd* pParent = NULL);	// standard constructor
    Ceditor m_editor;
	CString souce;
	bool ineditor;                 //判断是否击中编辑框
    int   tag;                      //判斷要画的是哪个东西
	CPoint pt;                       //点击的位置
	int width;                       //地图宽度
	int height;                        //地图高度
	

	// Dialog Data
	//{{AFX_DATA(CGameEditorDlg)
	enum { IDD = IDD_GAMEEDITOR_DIALOG };
	CTabCtrl	m_ToolsTab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton7();
	afx_msg void OnButton6();
	afx_msg void OnEdito();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEEDITORDLG_H__676710E1_8AAC_47E0_BED5_1F17B7D936BC__INCLUDED_)
