// YonExampleMFC.h : YonExampleMFC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CYonExampleMFCApp:
// �йش����ʵ�֣������ YonExampleMFC.cpp
//

class CYonExampleMFCApp : public CWinApp
{
public:
	CYonExampleMFCApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CYonExampleMFCApp theApp;