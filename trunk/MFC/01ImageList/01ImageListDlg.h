// 01ImageListDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include <vector>


// CMy01ImageListDlg �Ի���
class CMy01ImageListDlg : public CDialog
{
// ����
public:
	CMy01ImageListDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY01IMAGELIST_DIALOG };

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
	void enumFiles(const wchar_t* szSourceDir);
public:
	CListCtrl m_ListCtrl;
	CImageList m_ImageList;
	std::vector<CString> m_images;


	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
