// PropertyPage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "afxdlgs.h"


// CPropertyPageApp:
// �йش����ʵ�֣������ PropertyPage.cpp
//

class CPropertyPageApp : public CWinApp
{
public:
	CPropertyPageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	
};

extern CPropertyPageApp theApp;