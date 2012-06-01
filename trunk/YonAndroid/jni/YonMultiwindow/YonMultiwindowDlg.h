// YonMultiwindowDlg.h : ͷ�ļ�
//

#pragma once
#include "Dialog1.h"
#include "Dialog2.h"


// CYonMultiwindowDlg �Ի���
class CYonMultiwindowDlg : public CDialog
{
// ����
public:
	CYonMultiwindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;

// �Ի�������
	enum { IDD = IDD_YONMULTIWINDOW_DIALOG };

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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	CDialog1 m_dlg1;
	CDialog2 m_dlg2;
	
};
