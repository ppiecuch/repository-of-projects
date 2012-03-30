// YonExampleMFCView.cpp : CYonExampleMFCView 类的实现
//

#include "stdafx.h"
#include "YonExampleMFC.h"

#include "YonExampleMFCDoc.h"
#include "YonExampleMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYonExampleMFCView

IMPLEMENT_DYNCREATE(CYonExampleMFCView, CFormView)

BEGIN_MESSAGE_MAP(CYonExampleMFCView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CYonExampleMFCView 构造/析构

CYonExampleMFCView::CYonExampleMFCView()
	: CFormView(CYonExampleMFCView::IDD)
{
	// TODO: 在此处添加构造代码

}

CYonExampleMFCView::~CYonExampleMFCView()
{
}

void CYonExampleMFCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CYonExampleMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CYonExampleMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CYonExampleMFCView 诊断

#ifdef _DEBUG
void CYonExampleMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CYonExampleMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CYonExampleMFCDoc* CYonExampleMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYonExampleMFCDoc)));
	return (CYonExampleMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CYonExampleMFCView 消息处理程序

int CYonExampleMFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	HWND hWnd=this->GetSafeHwnd();
	if(hWnd==NULL)
	{
		AfxMessageBox(TEXT("获取渲染区窗口对象时HWND为空"));
		return -1;
	}

	SYonEngineParameters params;
	params.windowId=hWnd;
	engine=CreateEngine(params);

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();
	geometryFty=sceneMgr->getGeometryFactory();

	camera=sceneMgr->addCamera(core::vector3df(0,0,300));

	IMaterial* material;

	scene::IEntity* cube=geometryFty->createCube(core::dimension3df(50,50,50));
	cube1Model=sceneMgr->addModel(cube);
	material=cube1Model->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	cube1Model->setPosition(core::vector3df(20,40,0));
	material->setTexture(0,driver->getTexture("../media/test.png"));
	cube->drop();

	cube=geometryFty->createCube(core::dimension3df(100,100,100));
	cube2Model=sceneMgr->addModel(cube);
	material=cube2Model->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	cube2Model->setPosition(core::vector3df(-20,-20,0));
	material->setTexture(0,driver->getTexture("../media/earth.png"));
	cube->drop();

	scene::IEntity* plane=geometryFty->createXYPlane(core::dimension2df(50,50));
	planeModel=sceneMgr->addModel(plane);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(-20,20,100));
	material->setTexture(0,driver->getTexture("../media/aura.png"));
	plane->drop();



	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。

	return 0;
}

void CYonExampleMFCView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(WM_RENDER_FRAME);
	Logger->debug("CYonExampleMFCView::OnDestroy\n");
	engine->drop();
}
f32 factor=1.01f;
void CYonExampleMFCView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	if(engine->run())
	{
		//const core::vector3df pos=camera->getPosition();
		//camera->setPosition(core::vector3df(pos.x,pos.y+0.005f ,pos.z));

		const core::vector3df crot1=cube1Model->getRotation();
		cube1Model->setRotation(core::vector3df(crot1.x,crot1.y+0.05f ,crot1.z));

		const core::vector3df crot2=cube2Model->getRotation();
		cube2Model->setRotation(core::vector3df(crot2.x,crot2.y-0.05f ,crot2.z));

		//const core::vector3df crot3=cube3Model->getRotation();
		//cube3Model->setRotation(core::vector3df(crot3.x,crot3.y-0.05f ,crot3.z));

		const core::vector3df psca=planeModel->getScale();
		if(psca.x>4)
			factor=0.99f;
		else if(psca.x<2)
			factor=1.01f;
		planeModel->setScale(psca*factor);

		driver->begin(true,COLOR_BLACK);

		sceneMgr->render(driver);

		driver->end();
	}
}

BOOL CYonExampleMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

void CYonExampleMFCView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	engine->onResize(cx,cy);
}

void CYonExampleMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//更新窗口整个客户区域，同时重画时不擦除背景。
		InvalidateRect(NULL,FALSE);
	}

	CFormView::OnTimer(nIDEvent);
}
