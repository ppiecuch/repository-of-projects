// BarDemo.h : BarDemo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CBarDemoApp:
// �йش����ʵ�֣������ BarDemo.cpp
//

class CBarDemoApp : public CWinApp
{
public:
	CBarDemoApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBarDemoApp theApp;