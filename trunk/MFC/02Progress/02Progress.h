// 02Progress.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ProgressDlg.h"


// CMy02ProgressApp:
// �йش����ʵ�֣������ 02Progress.cpp
//

class CMy02ProgressApp : public CWinApp
{
public:
	CMy02ProgressApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy02ProgressApp theApp;