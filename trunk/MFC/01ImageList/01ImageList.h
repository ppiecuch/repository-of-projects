// 01ImageList.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy01ImageListApp:
// �йش����ʵ�֣������ 01ImageList.cpp
//

class CMy01ImageListApp : public CWinApp
{
public:
	CMy01ImageListApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy01ImageListApp theApp;