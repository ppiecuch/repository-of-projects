// Noninstall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNoninstallApp:
// �йش����ʵ�֣������ Noninstall.cpp
//

class CNoninstallApp : public CWinApp
{
public:
	CNoninstallApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNoninstallApp theApp;