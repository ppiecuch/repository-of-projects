// YonExampleSDIView.cpp : CYonExampleSDIView 类的实现
//

#include "stdafx.h"
#include "YonExampleSDI.h"

#include "YonExampleSDIDoc.h"
#include "YonExampleSDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYonExampleSDIView

IMPLEMENT_DYNCREATE(CYonExampleSDIView, CFormView)

BEGIN_MESSAGE_MAP(CYonExampleSDIView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CYonExampleSDIView 构造/析构

CYonExampleSDIView::CYonExampleSDIView()
	: CFormView(CYonExampleSDIView::IDD)
{
	// TODO: 在此处添加构造代码

}

CYonExampleSDIView::~CYonExampleSDIView()
{
}

void CYonExampleSDIView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CYonExampleSDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CYonExampleSDIView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CYonExampleSDIView 诊断

#ifdef _DEBUG
void CYonExampleSDIView::AssertValid() const
{
	CFormView::AssertValid();
}

void CYonExampleSDIView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CYonExampleSDIDoc* CYonExampleSDIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYonExampleSDIDoc)));
	return (CYonExampleSDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CYonExampleSDIView 消息处理程序

int CYonExampleSDIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	camera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	animatorFty=sceneMgr->getAnimatorFactory();

	IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	scene::IEntity* entity;

	/*shap=geometryFty->createSphere(80,64,64);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	sphereModel=sceneMgr->addModel(entity);
	material=sphereModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	//material->setPolygonMode(ENUM_POLYGON_MODE_LINE);
	sphereModel->setPosition(core::vector3df(100,100,0));
	material->setTexture(0,driver->getTexture("../media/earth.png"));
	shap->drop();
	unit->drop();
	entity->drop();*/

	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	cubeModel->setPosition(core::vector3df(50,50,200));
	material->setTexture(0,driver->getTexture("../media/test.png"));
	shap->drop();
	unit->drop();
	entity->drop();


	shap=geometryFty->createTorus(10,30,16,16,COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	toruseModel=sceneMgr->addModel(entity);
	material=toruseModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	material->setPolygonMode(ENUM_POLYGON_MODE_FILL);
	toruseModel->setPosition(core::vector3df(130,130,100));
	material->setTexture(0,driver->getTexture("../media/gunny.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(150,120,0));
	material->setTexture(0,driver->getTexture("../media/aura.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createXYRectangle2D2T(-25,-50,25,50,0,0,1,0.1f);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
	entity=geometryFty->createEntity(unit);
	IModel* waterfallModel=sceneMgr->addModel(entity);
	material=waterfallModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_MASK);
	waterfallModel->setPosition(core::vector3df(90,100,120));
	material->setTexture(0,driver->getTexture("../media/waterfall.png"));
	material->setTexture(1,driver->getTexture("../media/mask.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	SAnimatorParam aniParam;
	aniParam.type=ENUM_ANIMATOR_TYPE_UV;
	aniParam.animatorUV.unitIndex=0;
	aniParam.animatorUV.stage=0;
	IAnimator* uvAnimator=animatorFty->createAnimator(aniParam);
	waterfallModel->addAnimator(uvAnimator);
	uvAnimator->drop();


	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。

	return 0;
}

void CYonExampleSDIView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(WM_RENDER_FRAME);
	Logger->debug("CYonExampleMFCView::OnDestroy\n");
	engine->drop();
}

BOOL CYonExampleSDIView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}
f32 factor=1.01f;
void CYonExampleSDIView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	if(engine->run())
	{
		const core::vector3df crot=cubeModel->getRotation();
		cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

		//const core::vector3df srot=sphereModel->getRotation();
		//sphereModel->setRotation(core::vector3df(srot.x,srot.y-0.8f ,srot.z));

		const core::vector3df psca=planeModel->getScale();
		if(psca.x>4)
			factor=0.99f;
		else if(psca.x<2)
			factor=1.01f;
		planeModel->setScale(psca*factor);

		const core::vector3df trot=toruseModel->getRotation();
		toruseModel->setRotation(core::vector3df(trot.x+1.0f,trot.y,trot.z));

		driver->begin(true,true,video::SColor(0xFF132E47));

		sceneMgr->render(driver);

		Logger->drawString(driver,core::stringc("FPS:%d",driver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

		driver->end();
	}
}

void CYonExampleSDIView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	engine->onResize(cx,cy);
}

void CYonExampleSDIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//更新窗口整个客户区域，同时重画时不擦除背景。
		InvalidateRect(NULL,FALSE);
	}


	CFormView::OnTimer(nIDEvent);
}
