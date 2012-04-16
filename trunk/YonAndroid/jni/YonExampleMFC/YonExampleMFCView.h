// YonExampleMFCView.h : CYonExampleMFCView ��Ľӿ�
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
protected: // �������л�����
	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=20;
	CYonExampleMFCView();
	DECLARE_DYNCREATE(CYonExampleMFCView)

public:
	enum{ IDD = IDD_YONEXAMPLEMFC_FORM };

// ����
public:
	CYonExampleMFCDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CYonExampleMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // YonExampleMFCView.cpp �еĵ��԰汾
inline CYonExampleMFCDoc* CYonExampleMFCView::GetDocument() const
   { return reinterpret_cast<CYonExampleMFCDoc*>(m_pDocument); }
#endif

