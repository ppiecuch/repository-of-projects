// MultiView.h : MultiView Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "ITalicView.h"


// CMultiViewApp:
// �йش����ʵ�֣������ MultiView.cpp
//

class CMultiViewApp : public CWinApp
{
public:
	CMultiViewApp();
	// �����ĵ�ģ�����
	CMultiDocTemplate* m_pTemplateItalic;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();//����˳�ʵ������,���������ĵ�ģ�����
	
	
};

extern CMultiViewApp theApp;