// YonMFCDlg.h : ͷ�ļ�
//

#pragma once

#include "Dialog1.h"
#include "Dialog2.h"


// CYonMFCDlg �Ի���
class CYonMFCDlg : public CDialog
{
// ����
public:
	CYonMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YONMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CDialog1 m_dlg1;
	CDialog2 m_dlg2;
};
