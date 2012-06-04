// Dialog1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YonMultiwindow.h"
#include "Dialog1.h"

IModel* planeModel=NULL;
f32 factor=1.1f;
// CDialog1 �Ի���

IMPLEMENT_DYNAMIC(CDialog1, CDialog)

CDialog1::CDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog1::IDD, pParent)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog1, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialog1 ��Ϣ�������

int CDialog1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
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

	Logger->debug("Create Dialog1 engine\n");

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();

	geometryFty=sceneMgr->getGeometryFactory();

	camera=sceneMgr->addCamera(core::vector3df(0,0,300));
	animatorFty=sceneMgr->getAnimatorFactory();

	fs->setWorkingDirectory("../media/");

	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	scene::IEntity* entity;

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(0,0,0));
	material->setTexture(0,driver->getTexture("aura.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	return 0;
}

void CDialog1::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	engine->drop();
}

BOOL CDialog1::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CDialog1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if(engine->run())
	{
		driver->begin(true,true,video::SColor(0xFF132E47));

		const core::vector3df psca=planeModel->getScale();
		if(psca.x>4)
			factor= 0.9f;
		else if(psca.x<2)
			factor=1.1f;
		planeModel->setScale(psca*factor);

		sceneMgr->render(driver);

		Logger->drawString(driver,core::stringc("FPS:%d",driver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

		driver->end();
	}
}

void CDialog1::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	engine->onResize(cx,cy);
}
