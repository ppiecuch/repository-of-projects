// YonExampleDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CYonExampleDlgApp:
// �йش����ʵ�֣������ YonExampleDlg.cpp
//

class CYonExampleDlgApp : public CWinApp
{
	
public:
	CYonExampleDlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CYonExampleDlgApp theApp;