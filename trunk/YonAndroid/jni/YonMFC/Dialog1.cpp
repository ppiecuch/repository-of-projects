// Dialog1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YonMFC.h"
#include "Dialog1.h"


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
	ON_WM_TIMER()
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

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();

	geometryFty=sceneMgr->getGeometryFactory();

	camera=sceneMgr->addCamera(core::vector3df(0,0,300));
	animatorFty=sceneMgr->getAnimatorFactory();

	SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n �滻Ϊ����Ҫ��ʱ�����ڣ���λ���롣

	return 0;
}

void CDialog1::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(WM_RENDER_FRAME);
	Logger->debug("CYonExampleMFCView::OnDestroy\n");
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

		sceneMgr->render(driver);

		Logger->drawString(core::stringc("FPS:%d",driver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

		driver->end();
	}
}

void CDialog1::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	engine->onResize(cx,cy);
}

void CDialog1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==WM_RENDER_FRAME)
	{
		//���´��������ͻ�����ͬʱ�ػ�ʱ������������
		InvalidateRect(NULL,FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}
