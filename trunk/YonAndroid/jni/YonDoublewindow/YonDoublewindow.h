// YonDoublewindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYonDoublewindowApp:
// �йش����ʵ�֣������ YonDoublewindow.cpp
//

class CYonDoublewindowApp : public CWinApp
{
public:
	CYonDoublewindowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYonDoublewindowApp theApp;