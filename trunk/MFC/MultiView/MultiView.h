// MultiView.h : MultiView 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ITalicView.h"


// CMultiViewApp:
// 有关此类的实现，请参阅 MultiView.cpp
//

class CMultiViewApp : public CWinApp
{
public:
	CMultiViewApp();
	// 声明文档模板对象
	CMultiDocTemplate* m_pTemplateItalic;

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();//添加退出实例函数,用于销毁文档模板对象
	
	
};

extern CMultiViewApp theApp;