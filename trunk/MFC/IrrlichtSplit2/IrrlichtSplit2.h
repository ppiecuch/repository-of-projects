// IrrlichtSplit2.h : IrrlichtSplit2 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CIrrlichtSplit2App:
// �йش����ʵ�֣������ IrrlichtSplit2.cpp
//

class CIrrlichtSplit2App : public CWinApp
{
public:
	CIrrlichtSplit2App();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIrrlichtSplit2App theApp;