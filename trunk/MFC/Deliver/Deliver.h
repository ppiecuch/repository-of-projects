// Deliver.h : Deliver Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDeliverApp:
// �йش����ʵ�֣������ Deliver.cpp
//

class CDeliverApp : public CWinApp
{
public:
	CDeliverApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDeliverApp theApp;