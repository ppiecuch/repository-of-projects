#pragma once
#include "Context.h"


// CTabDialog �Ի���

class CTabDialog : public CDialog
{
	DECLARE_DYNAMIC(CTabDialog)

public:
	CTabDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_str;
	afx_msg LRESULT OnDownload(WPARAM wParam, LPARAM lParam);
};
