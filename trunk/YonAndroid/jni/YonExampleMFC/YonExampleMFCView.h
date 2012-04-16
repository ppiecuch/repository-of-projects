// YonExampleMFCView.h : CYonExampleMFCView 类的接口
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

#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>

#pragma comment(lib, "yon.lib")

//#include <vld.h>

class CYonExampleMFCView : public CFormView
{
protected: // 仅从序列化创建
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;
	CYonExampleMFCView();
	DECLARE_DYNCREATE(CYonExampleMFCView)

public:
	enum{ IDD = IDD_YONEXAMPLEMFC_FORM };

// 属性
public:
	CYonExampleMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CYonExampleMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	IModel* cubeModel;
	IModel* planeModel;
	IModel* toruseModel;
	//IModel* sphereModel;

	IYonEngine* engine;

	IVideoDriver* driver;
	ISceneManager* sceneMgr;
	IFileSystem* fs;
	IGeometryFactory* geometryFty;
	IAnimatorFactory*  animatorFty;
	ICamera* camera;

	bool initEGL(const HWND& hwnd);
	void destroyEGL();

	EGLDisplay m_eglDisplay;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;

	HDC m_hDc;
	HWND m_hWnd;
};

#ifndef _DEBUG  // YonExampleMFCView.cpp 中的调试版本
inline CYonExampleMFCDoc* CYonExampleMFCView::GetDocument() const
   { return reinterpret_cast<CYonExampleMFCDoc*>(m_pDocument); }
#endif

