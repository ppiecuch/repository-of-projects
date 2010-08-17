// COpenGLView.h : interface of the COpenGLView class
//
/////////////////////////////////////////////////////////////////////////////
#include "MyOpenGLFrameDoc.h"

// Include the OpenGL headers
#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glaux.h"
#define  PAI 3.1415926
#include "StdAfx.h"
#include "stdio.h"
struct WAVE 
{
	int center_wave_x,center_wave_y;
	int distance_wave;
	BOOL move;
}; 

class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Attributes
public:
	CMyOpenGLFrameDoc* GetDocument();

// Operations
public:
   	BOOL m_bFullView;
	GLuint texture[1];		// 存储一个纹理 (新增)将一改成二可以存两个
	float points[180][180][3];
	float xrot,yrot,zrot,wiggle_count;
	float wave[17];
     
	WAVE mywave;

    CList<WAVE,WAVE&>wavelist;
	



	int LoadGLTextures();
	AUX_RGBImageRec *LoadBMP(char *Filename);	// 载入位图图象
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// The following was added

	virtual BOOL SetupPixelFormat( void );
	virtual BOOL SetupViewport( int cx, int cy );
	virtual BOOL SetupViewingFrustum( GLdouble aspect_ratio );
	virtual BOOL SetupViewingTransform( void );
  	virtual BOOL PreRenderScene( void ) { return TRUE; }
 	virtual void RenderStockScene( void );
	virtual BOOL RenderScene( void );



private:
	BOOL InitializeOpenGL();
	void SetError( int e );


	HGLRC	m_hRC;
	CDC*	m_pDC;
	
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;

};

#ifndef _DEBUG  // debug version in COpenGLView.cpp
inline CMyOpenGLFrameDoc* COpenGLView::GetDocument()
   { return (CMyOpenGLFrameDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////





















