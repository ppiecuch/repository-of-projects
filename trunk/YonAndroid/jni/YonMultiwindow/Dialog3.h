#pragma once
#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;
using namespace yon::scene::animator;

#pragma comment(lib, "yon.lib")

// CDialog3 对话框

class CDialog3 : public CDialog
{
	DECLARE_DYNAMIC(CDialog3)

	const static UINT WM_RENDER_FRAME=3;
	const static UINT RENDER_INTERVAL=10;

public:
	CDialog3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialog3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

	IYonEngine* engine;

	IVideoDriver* driver;
	ISceneManager* sceneMgr;
	IFileSystem* fs;
	IGeometryFactory* geometryFty;
	IAnimatorFactory*  animatorFty;
	ICamera* camera;

	HWND m_hWnd;

	void setHWND(HWND hWnd)
	{
		m_hWnd=hWnd;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
