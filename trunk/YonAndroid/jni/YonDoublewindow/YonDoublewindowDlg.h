// YonDoublewindowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "RenderStatic.h"


// CYonDoublewindowDlg �Ի���
class CYonDoublewindowDlg : public CDialog
{
// ����
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;

public:
	CYonDoublewindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_YONDOUBLEWINDOW_DIALOG };

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
	//CStatic m_static1;
	//CStatic m_static2;
	CRenderStatic m_static1;
	CRenderStatic m_static2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
