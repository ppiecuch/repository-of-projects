// 33OneWindow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy33OneWindowApp:
// �йش����ʵ�֣������ 33OneWindow.cpp
//

class CMy33OneWindowApp : public CWinApp
{
public:
	CMy33OneWindowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy33OneWindowApp theApp;