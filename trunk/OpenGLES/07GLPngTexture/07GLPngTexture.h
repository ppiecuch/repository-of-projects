// 07GLPngTexture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy07GLPngTextureApp:
// �йش����ʵ�֣������ 07GLPngTexture.cpp
//

class CMy07GLPngTextureApp : public CWinApp
{
public:
	CMy07GLPngTextureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy07GLPngTextureApp theApp;