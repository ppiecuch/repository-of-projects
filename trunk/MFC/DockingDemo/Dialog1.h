#pragma once


// CDialog1 �Ի���
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
	CDialog1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialog1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
