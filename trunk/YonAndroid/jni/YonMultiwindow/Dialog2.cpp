// Dialog2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YonMultiwindow.h"
#include "Dialog2.h"

ISceneNode* planeModel2=NULL;
f32 factor2=1.1f;

// CDialog2 �Ի���

IMPLEMENT_DYNAMIC(CDialog2, CDialog)

CDialog2::CDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2::IDD, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDialog2 ��Ϣ�������

int CDialog2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//HWND hWnd=this->GetSafeHwnd();
	HWND hWnd=m_hWnd;
	if(hWnd==NULL)
	{
		AfxMessageBox(TEXT("��ȡ��Ⱦ�����ڶ���ʱHWNDΪ��"));
		return -1;
	}
	SYonEngineParameters params;
	params.windowId=hWnd;
	engine=CreateEngine(params);

	Logger->debug("Create Dialog1 engine\n");

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();

	geometryFty=sceneMgr->getGeometryFactory();

	camera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	animatorFty=sceneMgr->getAnimatorFactory();

	fs->addWorkingDirectory("../media/");

	//IMaterial* material;
	IShap *shap;
	IUnit* unit;
	scene::IEntity* entity;

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel2=sceneMgr->addSceneNode(entity);
	SMaterial& material=planeModel2->getMaterial(0);
	material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
	material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
	material.BlendDst=ENUM_BLEND_FACTOR_ONE;
	planeModel2->setPosition(core::vector3df(0,0,0));
	material.setTexture(0,driver->getTexture("aura00.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	//SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣


	return 0;
}

void CDialog2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	//KillTimer(WM_RENDER_FRAME);
	engine->drop();
}

BOOL CDialog2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CDialog2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if(engine->run())
	{
		driver->begin(true,true,video::SColor(0xFF132E47));

		const core::vector3df psca=planeModel2->getScale();
		if(psca.x>4)
			factor2= 0.9f;
		else if(psca.x<2)
			factor2=1.1f;
		planeModel2->setScale(psca*factor2);

		sceneMgr->render(driver);

		Logger->drawString(driver,core::stringc("FPS:%d",driver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

		driver->end();
	}

}

void CDialog2::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	engine->onResize(cx,cy);
}

void CDialog2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//InvalidateRect(NULL,FALSE);

	CDialog::OnTimer(nIDEvent);
}

BOOL CDialog2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);//openGL�����

	return CDialog::PreCreateWindow(cs);
}

void CDialog2::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACE("LD2\n");

	CDialog::OnLButtonDown(nFlags, point);
}
