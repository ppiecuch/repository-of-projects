// YonExampleMFCView.cpp : CYonExampleMFCView ���ʵ��
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

// CYonExampleMFCView ����/����

CYonExampleMFCView::CYonExampleMFCView()
	: CFormView(CYonExampleMFCView::IDD)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CYonExampleMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CYonExampleMFCView ���

#ifdef _DEBUG
void CYonExampleMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CYonExampleMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CYonExampleMFCDoc* CYonExampleMFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYonExampleMFCDoc)));
	return (CYonExampleMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CYonExampleMFCView ��Ϣ�������

int CYonExampleMFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	HWND hWnd=this->GetSafeHwnd();
	if(hWnd==NULL)
	{
		AfxMessageBox(TEXT("��ȡ��Ⱦ�����ڶ���ʱHWNDΪ��"));
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



	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣

	return 0;
}

void CYonExampleMFCView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(WM_RENDER_FRAME);
	Logger->debug("CYonExampleMFCView::OnDestroy\n");
	engine->drop();
}
f32 factor=1.01f;
void CYonExampleMFCView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}

void CYonExampleMFCView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	engine->onResize(cx,cy);
}

void CYonExampleMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//���´��������ͻ�����ͬʱ�ػ�ʱ������������
		InvalidateRect(NULL,FALSE);
	}

	CFormView::OnTimer(nIDEvent);
}
