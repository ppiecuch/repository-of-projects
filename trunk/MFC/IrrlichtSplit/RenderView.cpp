// RenderView.cpp : 实现文件
//

#include "stdafx.h"
#include "IrrlichtSplit.h"
#include "RenderView.h"


// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CFormView)

CRenderView::CRenderView()
	: CFormView(CRenderView::IDD)
{
	this->m_initial=false;
}

CRenderView::~CRenderView()
{
}

void CRenderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRenderView, CFormView)
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_RBUTTONDBLCLK()
//	ON_WM_CLOSE()
ON_WM_SIZE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEWHEEL()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()


// CRenderView 诊断

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRenderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRenderView 消息处理程序

void CRenderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	
	//context.hWnd = CreateWindow(L"BUTTON",L"",
	//	WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
	//	100, 0, 800, 600, this->GetSafeHwnd(), NULL, AfxGetInstanceHandle(), NULL);
	context.hWnd=this->GetSafeHwnd();
	context.pDoc=(CIrrlichtSplitDoc*)this->GetDocument();
	//context.view=this;
	m_thread=new RenderThread(context);
	m_thread->start();
	this->m_initial=true;

}
void CRenderView::DevRun(const Context& c)
{
	//while()
	//{
		// draw engine picture
		c.device->run();
		c.driver->beginScene(true, true, 0);
		c.smgr->drawAll();
		c.driver->endScene();
	//}
	//device->closeDevice();
	//if(device)
	//device->drop();
	//TRACE("drop device");
}


//void CRenderView::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	m_run=!m_run;
//	//if(m_run)
//	//	DevRun();
//	//CFormView::OnLButtonDblClk(nFlags, point);
//}


//void CRenderView::OnClose()
//{
//	TRACE("OnClose start");
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	MessageBox(L"here");
//	m_run=false;
//	CFormView::OnClose();
//	TRACE("OnClose end");
//}

LRESULT CRenderView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)   
    {   
    case WM_CLOSE:
       // MessageBox(_T("收到WM_CLOSE消息，即将调用DestroyWindow(hwnd);"),_T("注意了"),MB_OK);
        DestroyWindow();   
        break;   
    case WM_DESTROY:   
       // MessageBox(_T("已调用过DestroyWindow(hwnd)，看不到窗口了吧;"),_T("注意了"),MB_OK);   
       // MessageBox(_T("以下处理WM_DESTROY"),_T("注意了"),MB_OK);   
		this->m_thread->stop();
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		//MessageBox(_T("以下处理WM_SIZE"),_T("注意了"),MB_OK);   
		break;
    case WM_SYSCOMMAND:   
        if((LOWORD(wParam)&0xFFF0) == SC_CLOSE)   
        {
           // MessageBox(_T("你关闭了窗口,即将发送WM_CLOSE消息！"),_T("注意了"),MB_OK);   
            SendMessage(WM_CLOSE,0,0);   
        }
		break;
	default:
		break;
    }

	return CFormView::WindowProc(message, wParam, lParam);
}

void CRenderView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//一加入就出错,原因不明
	/*if(this->m_initial)
	{
		CString result,str;
		result=TEXT("以下处理WM_SIZE:");

		str.Format(L"%d",cx);
		result+=str+TEXT(",");
		str.Format(L"%d",cy);
		AfxMessageBox(result);
		this->m_thread->resize(cx,cy);
	}*/
	context.w=cx;
	context.h=cy;
}

void CRenderView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int x=point.x;
	int y=point.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
	irrevent.MouseInput.ButtonStates = 0;
    irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
    irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
	irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  
	//TRACE("post move event\r\n");
    context.device->postEventFromUser(irrevent);

	CFormView::OnMouseMove(nFlags, point);
}

void CRenderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x=point.x;
	int y=point.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
	irrevent.MouseInput.ButtonStates = 0;
    irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
    irrevent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN; 
	irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;
	//TRACE("post press event\r\n");
    context.device->postEventFromUser(irrevent);

	CFormView::OnLButtonDown(nFlags, point);
}

void CRenderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x=point.x;
	int y=point.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
	irrevent.MouseInput.ButtonStates = 0;
    irrevent.MouseInput.ButtonStates = irr::EMBSM_LEFT;
    irrevent.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
	irrevent.MouseInput.ButtonStates ^= irr::EMBSM_LEFT;  
	//TRACE("post release event\r\n");
    context.device->postEventFromUser(irrevent);
	this->m_thread->mouseReleased(x,y);
	CFormView::OnLButtonUp(nFlags, point);
}

BOOL CRenderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x=pt.x;
	int y=pt.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
	irrevent.MouseInput.Wheel=(short)(zDelta/120);
    irrevent.MouseInput.ButtonStates = irr::EMBSM_MIDDLE;
    irrevent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
	TRACE("post wheel event:%d\r\n",zDelta);
    context.device->postEventFromUser(irrevent);

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CRenderView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x=point.x;
	int y=point.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
	irrevent.MouseInput.ButtonStates = 0;
    irrevent.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN; 
	irrevent.MouseInput.ButtonStates = irr::EMBSM_RIGHT;
	//TRACE("post right press event\r\n");
    context.device->postEventFromUser(irrevent);

	CFormView::OnLButtonDown(nFlags, point);
}

void CRenderView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x=point.x;
	int y=point.y;

	SEvent irrevent;
	//MouseInput.ButtonStates
	//A bitmap of button states. 
	//You can use isButtonPressed() to determine if a button is pressed or not. 
	//Currently only valid if the event was EMIE_MOUSE_MOVED 
    irrevent.MouseInput.ButtonStates = 0xffffffff;
	//EEVENT_TYPE irr::SEvent::EventType 
	//Enumeration for all event types there are. 
    irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
	//EMOUSE_INPUT_EVENT irr::SEvent::SMouseInput::Event 
	//Type of mouse event.
    irrevent.MouseInput.Event = irr::EMIE_COUNT;
	irrevent.MouseInput.X = x;
	irrevent.MouseInput.Y = y;
    irrevent.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
	irrevent.MouseInput.ButtonStates = irr::EMBSM_RIGHT;
	//TRACE("post right release event\r\n");
    context.device->postEventFromUser(irrevent);

	CFormView::OnLButtonUp(nFlags, point);
}

void CRenderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	irr::EKEY_CODE key;
	switch(nChar)
	{
	case 'W':
		key = irr::KEY_UP;
		break;
	case 'S':
		key = irr::KEY_DOWN;
		break;
	case 'A':
		key = irr::KEY_LEFT;
		break;
	case 'D':
		key = irr::KEY_RIGHT;
		break;
	default:
		return;
	}
	SEvent irrevent;
	irrevent.KeyInput.Key = key;
	irrevent.EventType = irr::EET_KEY_INPUT_EVENT;
	irrevent.KeyInput.Char = nChar;
	irrevent.KeyInput.PressedDown = true;
	context.device->postEventFromUser(irrevent);
	//TRACE("post key down event\r\n");
	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRenderView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	irr::EKEY_CODE key;
	switch(nChar)
	{
	case 'W':
		key = irr::KEY_UP;
		break;
	case 'S':
		key = irr::KEY_DOWN;
		break;
	case 'A':
		key = irr::KEY_LEFT;
		break;
	case 'D':
		key = irr::KEY_RIGHT;
		break;
	default:
		return;
	}
	SEvent irrevent;
	irrevent.KeyInput.Key = key;
	irrevent.EventType = irr::EET_KEY_INPUT_EVENT;
	irrevent.KeyInput.Char = nChar;
	irrevent.KeyInput.PressedDown = false;
	context.device->postEventFromUser(irrevent);
	//TRACE("post key up event\r\n");
	CFormView::OnKeyUp(nChar, nRepCnt, nFlags);
}
