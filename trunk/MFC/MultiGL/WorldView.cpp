#include "stdafx.h"
#include "WorldView.h"
#include <gl\GL.h>
#include <gl\GLU.h>
using namespace Run;
#pragma comment( lib,"opengl32.lib")
#pragma comment( lib,"glu32.lib")
using namespace Run;
HRESULT WINAPI WorldViewCallBack(HWND hWnd,UINT msg,LPARAM lParma,WPARAM wParam);
List<WorldView,4,4> ViewList;
UINT_PTR            TimerId=0;
bool isReg       =false;
bool DILStarted  =false;
HRESULT RegClass(HINSTANCE hInst);
WorldView::WorldView(int tx,int ty,unsigned int twidth,unsigned int theight,HINSTANCE instance)
{
	this->alpha=255;
	this->hWnd=0;
	this->hdc=0;
	this->glrc=0;
	this->EventCall=0;
	this->bgcolor[0]=0;
	this->bgcolor[1]=0;
	this->bgcolor[2]=0;
	this->keycolor[0]=255;
	this->keycolor[1]=255;
	this->keycolor[2]=255;
	if(!isReg) 
	{
		if(RegClass(instance)) isReg  =true;
	}
	if(!DILStarted)
	{
		//if(StartDevIL()) DILStarted   =true;
	}
	this->left=tx;
	this->top =ty;
	this->width=twidth;
	this->height=theight;
	this->DragL=0;
	this->DragT=0;
	this->DragR=this->width;
	this->DragB=this->height;
	this->hWnd=CreateWindow(L"WorldView",L"RunUI-Toolkit-WorldView",WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_MINIMIZEBOX|WS_VISIBLE|WS_CHILD,tx,ty,this->width,this->height,GetDesktopWindow(),NULL,instance,NULL);
	if(this->hWnd)
	{
		DWORD dwExStyle = GetWindowLong(this->hWnd,GWL_EXSTYLE);  
		SetWindowLong(this->hWnd,GWL_EXSTYLE,dwExStyle^0x80000);
		SetLayeredWindowAttributes(this->hWnd,RGB(this->keycolor[0],this->keycolor[1],this->keycolor[2]),this->alpha,ULW_ALPHA|ULW_COLORKEY);
		HRGN rgn=CreateRoundRectRgn(0,0,this->width,this->height,5,5);
		SetWindowRgn(this->hWnd,rgn,true);
		this->hdc=GetDC(this->hWnd);
		ShowWindow(this->hWnd,SW_SHOW);
		UpdateWindow(this->hWnd);
	}
	ViewList.AddElement(this);
	this->StartGL();
}
WorldView::~WorldView()
{
	wglDeleteContext(this->glrc);
	DestroyWindow(this->hWnd);
}
int WorldView::StartGL()
{

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR );
	pfd.nVersion = 3;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32; 
	pfd.cDepthBits = 24;
	pfd.cAlphaBits = 8;
	SetPixelFormat(this->hdc, ChoosePixelFormat(this->hdc,&pfd ),&pfd); 
	this->glrc=wglCreateContext(this->hdc);
	wglMakeCurrent(this->hdc,this->glrc);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();					
	gluPerspective(90.0f,static_cast<float>(width)/static_cast<float>(height), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();						
	gluLookAt(0,0,1.0,0,0,0,0.0f,1.0f,0);
	glShadeModel(GL_SMOOTH);  
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	GLint bufs, samples;  
	if(!this->glrc) return -1;
	return 1;
}
int  WorldView::Show()
{
	return ShowWindow(this->hWnd,SW_SHOW);
}
int  WorldView::Hide()
{
	return ShowWindow(this->hWnd,SW_HIDE);
}
char WorldView::GetAlpha()
{
	return this->alpha;
}
HWND WorldView::GetHwnd()
{
	return this->hWnd;
}
int WorldView::SetFatherByHwnd(HWND thwnd)
{
	SetParent(this->hWnd,thwnd);
	return static_cast<int>(GetLastError());
}
int WorldView::SetFather(WorldView* father)
{
	SetParent(this->hWnd,father->GetHwnd());
	return static_cast<int>(GetLastError());
}
mlPoint  WorldView::GetPosition()
{
	mlPoint point;
	point.x=this->left;
	point.y=this->top;
	return point;
}
mlSize   WorldView::GetSize()
{
	mlSize size;
	size.width=this->width;
	size.height=this->height;
	return size;
}
int WorldView::SetAlpha(char talpha)
{
	this->alpha=talpha;
	return SetLayeredWindowAttributes (this->hWnd,RGB(this->keycolor[0],this->keycolor[1],this->keycolor[2]),this->alpha,ULW_ALPHA|ULW_COLORKEY);
}
int WorldView::SetPosition(int tx,int ty)
{
	this->left=tx;
	this->top=ty;
	return MoveWindow(this->hWnd,this->left,this->top,this->width,this->height,true);
}
int WorldView::SetPosition(mlPoint tpoint)
{
	this->left=tpoint.x;
	this->top =tpoint.y;
	return MoveWindow(this->hWnd,this->left,this->top,this->width,this->height,true);
}
int WorldView::SetSize(unsigned int twidth,unsigned int theight)
{
	this->width=twidth;
	this->height=theight;
	MoveWindow(this->hWnd,this->left,this->top,this->width,this->height,true);
    HRGN rgn=CreateRoundRectRgn(0,0,this->width,this->height,5,5);
	SetWindowRgn(this->hWnd,rgn,true);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();					
	gluPerspective(90.0f,static_cast<float>(width)/static_cast<float>(height), 1.0, 150.0);
	return 1;
}
int WorldView::SetSize(mlSize tsize)
{
	this->width=tsize.width;
	this->height=tsize.height;
	MoveWindow(this->hWnd,this->left,this->top,this->width,this->height,true);
	HRGN rgn=CreateRoundRectRgn(0,0,this->width,this->height,5,5);
	SetWindowRgn(this->hWnd,rgn,true);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();					
	gluPerspective(90.0f,static_cast<float>(width)/static_cast<float>(height), 1.0, 150.0);
	return 1;
}
int WorldView::SetBackgroundImage(wchar_t* filename)
{
	return 1;
}
int WorldView::SetBackgroundColor(unsigned char r,unsigned char g,unsigned char b)
{
	this->bgcolor[0]=r;
	this->bgcolor[1]=g;
	this->bgcolor[2]=b;
	return 1;
}
int WorldView::SetKeyColor(unsigned char r,unsigned char g,unsigned char b)
{
	this->keycolor[0]=r;
	this->keycolor[1]=g;
	this->keycolor[2]=b;
	return SetLayeredWindowAttributes (this->hWnd,RGB(this->keycolor[0],this->keycolor[1],this->keycolor[2]),this->alpha,ULW_ALPHA|ULW_COLORKEY);
}
int WorldView::SetDragRegion(int x1,int y1,int x2,int y2)
{
	this->DragL=x1;
	this->DragT=y1;
	this->DragR=x2;
	this->DragB=y2;
	return 1;
}
int WorldView::Render()
{
	wglMakeCurrent(this->hdc,this->glrc);
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();
	gluLookAt(0,0,1.0,0,0,0,0.0f,1.0f,0);
	glClearColor(this->bgcolor[0]/255,this->bgcolor[1]/255,this->bgcolor[2]/255,this->alpha/255);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	float light_ambient[] = {1.0,1.0,1.0,1.0};
	float light_diffuse[] = {0.0,0.0,0.0,1.0};
	float light_specular[] = {0.0,0.0,0.0,1.0};
	float light_position[] = {0.0,0.0,0.0,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glFlush();
	SwapBuffers(this->hdc);
	Sleep(20);
	return 1;
}
int WorldView::SetEventCall(UiCall callback)
{
	this->EventCall=callback;
	return 1;
}
int WorldView::CallUser(Event tevent)
{
	if(this->EventCall) return this->EventCall(tevent);
	return -1;
}
HRESULT WINAPI WorldViewCallBack(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	WorldView* CurrentView=0;
	for(int i=0;i!=ViewList.GetElementNumber();i++)
	{
		if(hWnd==ViewList[i]->GetHwnd())
		{
			CurrentView=ViewList[i];
			break;
		}
	}
	PAINTSTRUCT LP;
	Event tevent={0};
	if(CurrentView)
	{
		tevent.hWnd=CurrentView->GetHwnd();
		tevent.View=CurrentView;
		tevent.x=MAKEPOINTS(lParam).x;
		tevent.y=MAKEPOINTS(lParam).y;
	}
	switch(msg)
	{
	case WM_CREATE:
		SetTimer(hWnd,ViewList.GetElementNumber(),40,NULL);
		break;
	case WM_TIMER:
		if(CurrentView)
		{
			tevent.EventClass=Event::Render;
			CurrentView->CallUser(tevent);
			CurrentView->Render();
		}
		break;
	case WM_MOVE:
		if(CurrentView)
		{
			CurrentView->left=MAKEPOINTS(lParam).x;
			CurrentView->top =MAKEPOINTS(lParam).y;
		}
		break;
	case WM_PAINT:
		BeginPaint(hWnd,&LP);
		if(CurrentView) 
		{
			CurrentView->Render();
		}
		EndPaint(hWnd,&LP);
		break;
	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_LBUTTONDOWN:
		if(CurrentView) 
		{
			if(tevent.x>CurrentView->DragL&&tevent.x<CurrentView->DragR&&tevent.y>CurrentView->DragT&&tevent.y<CurrentView->DragB)
			{
				SendMessage(hWnd,WM_NCLBUTTONDOWN ,HTCAPTION,0);
			}
			else
			{
				tevent.EventClass=Event::Clicked;
				CurrentView->CallUser(tevent);
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_IME_CHAR:
		break;
	case WM_DESTROY:
		for(int i=0;i!=ViewList.GetElementNumber();i++)
			{
				if(hWnd==ViewList[i]->GetHwnd())
				{
					ViewList.DeleteElement(i);
					break;
				}
			}
		if(!ViewList.GetElementNumber()) PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
		break;
	}
	return 0;
}
HRESULT RegClass(HINSTANCE hInst)
{
	WNDCLASSEX wcx;  
    wcx.cbSize = sizeof(wcx);         
	wcx.style = CS_DBLCLKS|CS_OWNDC;        
    wcx.lpfnWndProc =WorldViewCallBack;    
    wcx.cbClsExtra = 0;               
    wcx.cbWndExtra = 0;          
    wcx.hInstance =hInst;        
    wcx.hIcon = LoadIcon(NULL,IDI_APPLICATION);             
    wcx.hCursor = LoadCursor(NULL,IDC_ARROW);                   
    wcx.hbrBackground =NULL;     
    wcx.lpszMenuName = NULL;  
    wcx.lpszClassName =L"WorldView"; 
    wcx.hIconSm = NULL;
    return RegisterClassEx(&wcx); 
}
