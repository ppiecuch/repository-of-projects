// IrrlichtSplit.h : IrrlichtSplit Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CIrrlichtSplitApp:
// �йش����ʵ�֣������ IrrlichtSplit.cpp
//

class CIrrlichtSplitApp : public CWinApp
{
public:
	CIrrlichtSplitApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIrrlichtSplitApp theApp;