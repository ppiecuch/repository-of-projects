// PropertyPageDlg.h : ͷ�ļ�
//

#pragma once
#include "page1.h"
#include "page2.h"


// CPropertyPageDlg �Ի���
class CPropertyPageDlg : public CDialog
{
// ����
public:
	CPropertyPageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROPERTYPAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CPropertySheet nsheet;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
