// Nesting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Child.h"


// CNestingApp:
// �йش����ʵ�֣������ Nesting.cpp
//

class CNestingApp : public CWinApp
{
public:
	CNestingApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNestingApp theApp;