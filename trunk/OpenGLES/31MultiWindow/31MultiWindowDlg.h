// 31MultiWindowDlg.h : ͷ�ļ�
//

#pragma once

#include "Dialog1.h"
#include "Dialog2.h"


// CMy31MultiWindowDlg �Ի���
class CMy31MultiWindowDlg : public CDialog
{
// ����
public:
	CMy31MultiWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY31MULTIWINDOW_DIALOG };

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
