// DialogBar.h : DialogBar Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDialogBarApp:
// �йش����ʵ�֣������ DialogBar.cpp
//

class CDialogBarApp : public CWinApp
{
public:
	CDialogBarApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDialogBarApp theApp;