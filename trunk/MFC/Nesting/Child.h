#pragma once


// CChild �Ի���

class CChild : public CDialog
{
	DECLARE_DYNAMIC(CChild)

public:
	CChild(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChild();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
