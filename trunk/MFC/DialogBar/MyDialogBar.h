#pragma once


// CMyDialogBar �Ի���

class CMyDialogBar : public CDialog
{
	DECLARE_DYNAMIC(CMyDialogBar)

public:
	CMyDialogBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDialogBar();

// �Ի�������
	enum { IDD = IDD_DIALOGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
