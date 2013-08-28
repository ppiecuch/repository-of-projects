// CxImageDlg.h : ͷ�ļ�
//

#pragma once

#include <locale.h>
#include "afxwin.h"


#include <ximage.h>
#pragma comment( lib, "lib/cximage.lib")
#pragma comment( lib, "lib/libdcr.lib")
#pragma comment( lib, "lib/libpsd.lib")
#pragma comment( lib, "lib/mng.lib")
#pragma comment( lib, "lib/jbig.lib")
#pragma comment( lib, "lib/Jpeg.lib")
#pragma comment( lib, "lib/png.lib")
#pragma comment( lib, "lib/Tiff.lib")
#pragma comment( lib, "lib/zlib.lib")
#pragma comment( lib, "lib/jasper.lib")

#define PICTURE_WIDTH 350
#define PICTURE_HEIGHT 250



// CCxImageDlg �Ի���
class CCxImageDlg : public CDialog
{
// ����
public:
	CCxImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CXIMAGE_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CStatic picture;
};
