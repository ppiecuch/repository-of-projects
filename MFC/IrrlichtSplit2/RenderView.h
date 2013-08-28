#pragma once

#pragma comment(lib,"irrlicht.lib")
#include <irrlicht.h>
#include "IrrlichtSplitDoc.h"
#include "EditorCamera.h"

using namespace irr;
using namespace io;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;



// CRenderView 窗体视图
enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};
class CRenderView : public CFormView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	CRenderView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRenderView();

	const static UINT WM_RENDER_FRAME=1;
	const static UINT RENDER_INTERVAL=50;

public:
	enum { IDD = IDD_DIALOG_RENDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	void InitIrrlicht(void);
	void ResizeIrrlicht(int cx, int cy);
	void DrawIrrlicht(void);
	void DestroyIrrlicht(void);

	void mouseReleased(int x,int y);
	void initEditing(void);

	IrrlichtDevice* device;
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;
	scene::ICameraSceneNode* camera;
	SIrrlichtCreationParameters param;
	scene::ISceneNodeAnimator* anim;
	gui::IGUIEnvironment* env;
	CIrrlichtSplitDoc* pDoc;

	ITerrainSceneNode* terrain;

	ISceneCollisionManager* collMan;
	core::vector3df intersection;
	core::triangle3df hitTriangle;
	core::line3d<f32> ray;
	
	//CEditing m_editing;
};


