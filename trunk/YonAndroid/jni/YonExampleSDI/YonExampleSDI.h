// YonExampleSDI.h : YonExampleSDI Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CYonExampleSDIApp:
// �йش����ʵ�֣������ YonExampleSDI.cpp
//

class CYonExampleSDIApp : public CWinApp
{
public:
	CYonExampleSDIApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CYonExampleSDIApp theApp;