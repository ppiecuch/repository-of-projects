// 06GLBmpTextureDlg.h : ͷ�ļ�
//

#pragma once
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"glut32.lib")
#include <GL/glew.h>
#include "gl\gl.h"
#include "gl\glu.h"
#include <GL/glut.h>



// CMy06GLBmpTextureDlg �Ի���
class CMy06GLBmpTextureDlg : public CDialog
{
// ����
public:
	CMy06GLBmpTextureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY06GLBMPTEXTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	int m_GLPixelIndex;
	bool SetWindowPixelFormat(HDC hdc);
	bool CreateViewGLContext(HDC hDC);
	//HGLRC��һ��ָ��rendering context�ľ���� 
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
