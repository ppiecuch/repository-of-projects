#if !defined(AFX_SETMAP_H__6111794F_2339_4FD4_9AD7_3AE731BC85D0__INCLUDED_)
#define AFX_SETMAP_H__6111794F_2339_4FD4_9AD7_3AE731BC85D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetMap dialog

class CSetMap : public CDialog
{
// Construction
public:
	CSetMap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetMap)
	enum { IDD = IDD_DIALOG1 };
	int		m_width;
	int		m_height;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetMap)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETMAP_H__6111794F_2339_4FD4_9AD7_3AE731BC85D0__INCLUDED_)
