// SDIFT2.h : SDIFT2 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSDIFT2App:
// �йش����ʵ�֣������ SDIFT2.cpp
//

class CSDIFT2App : public CWinApp
{
public:
	CSDIFT2App();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSDIFT2App theApp;