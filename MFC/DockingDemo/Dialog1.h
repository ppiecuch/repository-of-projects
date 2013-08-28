#pragma once


// CDialog1 对话框
/*
class TabDialog : public CDialog
{private:
DECLARE_MESSAGE_MAP()
virtual void OnCancel();
virtual void OnOK();
};*/

class CDialog1 : public CDialog,public TabCtrlNotify
{
	DECLARE_DYNAMIC(CDialog1)

public:
	CDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialog1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
