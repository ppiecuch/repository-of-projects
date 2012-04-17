// YonExampleSDIView.h : CYonExampleSDIView 类的接口
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

#pragma comment(lib, "yon.lib")


class CYonExampleSDIView : public CFormView
{
protected: // 仅从序列化创建
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;
	CYonExampleSDIView();
	DECLARE_DYNCREATE(CYonExampleSDIView)

public:
	enum{ IDD = IDD_YONEXAMPLESDI_FORM };

// 属性
public:
	CYonExampleSDIDoc* GetDocument() const;

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
	virtual ~CYonExampleSDIView();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
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
};

#ifndef _DEBUG  // YonExampleSDIView.cpp 中的调试版本
inline CYonExampleSDIDoc* CYonExampleSDIView::GetDocument() const
   { return reinterpret_cast<CYonExampleSDIDoc*>(m_pDocument); }
#endif

