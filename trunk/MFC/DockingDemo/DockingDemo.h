// DockingDemo.h : DockingDemo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDockingDemoApp:
// �йش����ʵ�֣������ DockingDemo.cpp
//

class CDockingDemoApp : public CWinApp
{
public:
	CDockingDemoApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDockingDemoApp theApp;