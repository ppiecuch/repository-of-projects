// Toolbar.h : Toolbar Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CToolbarApp:
// �йش����ʵ�֣������ Toolbar.cpp
//

class CToolbarApp : public CWinApp
{
public:
	CToolbarApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CToolbarApp theApp;