// YonExampleDlgDlg.h : 头文件
//

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

//memory leak?
//refer to:http://www.vis-sim.com/3dsceneBB/viewtopic.php?t=1027
//出现msvcrtd.lib(crtexew.obj) : error LNK2001: unresolved external symbol _WinMain@1
//将程序入口点设置为wWinMainCRTStartup（链接-》高级-》入口点）
#pragma comment(lib, "yon.lib")

// CYonExampleDlgDlg 对话框
class CYonExampleDlgDlg : public CDialog
{
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;
// 构造
public:
	CYonExampleDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_YONEXAMPLEDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	IYonEngine* engine;

	IVideoDriver* driver;
	ISceneManager* sceneMgr;
	IFileSystem* fs;
	IGeometryFactory* geometryFty;
	IAnimatorFactory*  animatorFty;
	ICamera* camera;

	IModel* cubeModel;
	IModel* planeModel;
	IModel* toruseModel;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
