// ColorSelector.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CColorSelectorApp:
// �йش����ʵ�֣������ ColorSelector.cpp
//

class CColorSelectorApp : public CWinApp
{
public:
	CColorSelectorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CColorSelectorApp theApp;