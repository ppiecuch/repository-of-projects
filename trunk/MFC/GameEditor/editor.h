#if !defined(AFX_EDITOR_H__4CCA7DC5_C0F6_4AAB_9427_99340AFA98BF__INCLUDED_)
#define AFX_EDITOR_H__4CCA7DC5_C0F6_4AAB_9427_99340AFA98BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Ceditor dialog

class Ceditor : public CDialog
{
// Construction
public:
	Ceditor(CWnd* pParent = NULL);   // standard constructor
	CString s;
    CString s2;
	CRect Real_rect2;    //Real_rect2����Ԥ�����ľ���
	CRect Real_rect;     //Real_rect ����ѡͼ���ľ���
	bool  r1;            //�Д�ѡͼ���Ƿ�ѡ��
	bool  r2;            //�ж�Ԥ�����Ƿ�ѡ��
	CPoint pt;           //���ֵ���ѡͼ����λ��
// Dialog Data
	//{{AFX_DATA(Ceditor)
	enum { IDD = IDD_EDITOR };
	CComboBox	m_combox2;
	CComboBox	m_combox1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Ceditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Ceditor)
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnPreview();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITOR_H__4CCA7DC5_C0F6_4AAB_9427_99340AFA98BF__INCLUDED_)
