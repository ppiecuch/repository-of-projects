// YonMultiwindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYonMultiwindowApp:
// �йش����ʵ�֣������ YonMultiwindow.cpp
//

class CYonMultiwindowApp : public CWinApp
{
public:
	CYonMultiwindowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYonMultiwindowApp theApp;