// 06GLBmpTexture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy06GLBmpTextureApp:
// �йش����ʵ�֣������ 06GLBmpTexture.cpp
//

class CMy06GLBmpTextureApp : public CWinApp
{
public:
	CMy06GLBmpTextureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy06GLBmpTextureApp theApp;