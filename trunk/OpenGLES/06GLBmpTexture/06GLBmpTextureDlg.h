// 06GLBmpTextureDlg.h : 头文件
//

#pragma once
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"glut32.lib")
#include <GL/glew.h>
#include "gl\gl.h"
#include "gl\glu.h"
#include <GL/glut.h>



// CMy06GLBmpTextureDlg 对话框
class CMy06GLBmpTextureDlg : public CDialog
{
// 构造
public:
	CMy06GLBmpTextureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY06GLBMPTEXTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	int m_GLPixelIndex;
	bool SetWindowPixelFormat(HDC hdc);
	bool CreateViewGLContext(HDC hDC);
	//HGLRC是一个指向rendering context的句柄。 
	HGLRC m_hGLContext;
	HDC hDC;
	void GetGLInfo(void);
	bool InitGL(void);
	void DrawGLScene(void);
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
