// SpinCtrl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSpinCtrlApp:
// �йش����ʵ�֣������ SpinCtrl.cpp
//

class CSpinCtrlApp : public CWinApp
{
public:
	CSpinCtrlApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSpinCtrlApp theApp;