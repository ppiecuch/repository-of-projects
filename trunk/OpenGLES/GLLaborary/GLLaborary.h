// GLLaborary.h : GLLaborary Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CGLLaboraryApp:
// �йش����ʵ�֣������ GLLaborary.cpp
//

class CGLLaboraryApp : public CWinApp
{
public:
	CGLLaboraryApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGLLaboraryApp theApp;