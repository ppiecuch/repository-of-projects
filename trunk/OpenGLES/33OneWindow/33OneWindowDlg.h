// 33OneWindowDlg.h : ͷ�ļ�
//

#pragma once
#include "Dialog1.h"

// CMy33OneWindowDlg �Ի���
class CMy33OneWindowDlg : public CDialog
{
// ����
public:
	CMy33OneWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY33ONEWINDOW_DIALOG };

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
};
