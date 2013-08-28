// CxImageDlg.h : 头文件
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



// CCxImageDlg 对话框
class CCxImageDlg : public CDialog
{
// 构造
public:
	CCxImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CXIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CStatic picture;
};
