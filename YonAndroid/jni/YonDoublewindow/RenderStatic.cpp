// RenderStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "YonDoublewindow.h"
#include "RenderStatic.h"


// CRenderStatic


IMPLEMENT_DYNAMIC(CRenderStatic, CStatic)

CRenderStatic::CRenderStatic():planeModel(NULL),factor(1.1f)
{

}

CRenderStatic::~CRenderStatic()
{
}


BEGIN_MESSAGE_MAP(CRenderStatic, CStatic)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CRenderStatic 消息处理程序



int CRenderStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CRenderStatic::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//KillTimer(WM_RENDER_FRAME);
	engine->drop();
	delete params.pEventReceiver;
}

BOOL CRenderStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CStatic::OnEraseBkgnd(pDC);
	return TRUE;
}

void CRenderStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
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

void CRenderStatic::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	engine->onResize(cx,cy);
}

void CRenderStatic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//InvalidateRect(NULL,FALSE);

	CStatic::OnTimer(nIDEvent);
}
class MyEventReceiver : public event::IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				Logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				Logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				return true;
			}
		}
		return false;
	}
};
void CRenderStatic::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	HWND hWnd=this->GetSafeHwnd();
	if(hWnd==NULL)
	{
		AfxMessageBox(TEXT("获取渲染区窗口对象时HWND为空"));
		return;
	}
	
	params.windowId=hWnd;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	Logger->debug("Create engine\n");

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	fs=engine->getFileSystem();

	geometryFty=sceneMgr->getGeometryFactory();

	camera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	animatorFty=sceneMgr->getAnimatorFactory();

	fs->addWorkingDirectory("../media/");

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

	//SetTimer(WM_RENDER_FRAME,RENDER_INTERVAL,NULL);// n 替换为你需要定时的周期，单位毫秒。


	CStatic::PreSubclassWindow();
}

BOOL CRenderStatic::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);//openGL必需的
	TRACE("PreCreateWindow\n");

	return CStatic::PreCreateWindow(cs);
}

void CRenderStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//TRACE("KD\n");
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRenderStatic::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("KU\n");
	CStatic::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CRenderStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("LD:%d\n",nFlags);
	event::SEvent evt;
	evt.type=event::ENUM_EVENT_TYPE_MOUSE;
	evt.mouseInput.type=event::ENUM_MOUSE_INPUT_TYPE_LDOWN;
	evt.mouseInput.buttonMasks=event::MASK_MOUSE_BUTTON_LEFT;
	evt.mouseInput.x=point.x;
	evt.mouseInput.y=point.y;
	engine->postEventFromUser(evt);
	CStatic::OnLButtonDown(nFlags, point);
}

void CRenderStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("LU\n");
	event::SEvent evt;
	evt.type=event::ENUM_EVENT_TYPE_MOUSE;
	evt.mouseInput.type=event::ENUM_MOUSE_INPUT_TYPE_LUP;
	evt.mouseInput.buttonMasks=event::MASK_MOUSE_BUTTON_LEFT;
	evt.mouseInput.x=point.x;
	evt.mouseInput.y=point.y;
	engine->postEventFromUser(evt);
	CStatic::OnLButtonUp(nFlags, point);
}
