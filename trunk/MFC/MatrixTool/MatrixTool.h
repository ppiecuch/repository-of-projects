// MatrixTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMatrixToolApp:
// �йش����ʵ�֣������ MatrixTool.cpp
//

class CMatrixToolApp : public CWinApp
{
public:
	CMatrixToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMatrixToolApp theApp;