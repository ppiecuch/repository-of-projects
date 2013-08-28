#include "stdafx.h"
#ifndef _WorldView_H_
#define _WorldView_H_
#include <Windows.h>
#include "TypeDefine.h"
#include "mlList.h"
namespace Run
{
class WorldView;
typedef struct _Event
{
	enum EventType{Clicked=1,DClicked,Timer,Render,PositionChanged,SizeChanged,TextChanged,Destroyed};
	unsigned int Type;
	WorldView*   View;
	unsigned int EventClass;
	int x;
	int y;
	HWND hWnd;
}Event;
typedef  int   (__stdcall *UiCall)(Event tevent);
class WorldView
{
public:
	int               top;
	int               left;
	int               DragL;
	int               DragR;
	int               DragT;
	int               DragB;
	WorldView(int tx,int ty,unsigned int twidth,unsigned int theight,HINSTANCE instance);
   ~WorldView();
	HWND GetHwnd();
	int  Show();
	int  Hide();
	int  SetFatherByHwnd(HWND hWnd);
	int  SetFather(WorldView* father);
	int  SetAlpha(char talpha);
	char GetAlpha();
	int  SetPosition(int tx,int ty);
	int  SetPosition(mlPoint tpoint);
	mlPoint GetPosition();
	int  SetSize(unsigned int twidth,unsigned int theight);
	int  SetSize(mlSize tsize);
	int  SetKeyColor(unsigned char r,unsigned char g,unsigned char b);
	int  SetBackgroundColor(unsigned char r,unsigned char g,unsigned char b);
	int  SetBackgroundImage(wchar_t* filename);
	int  SetDragRegion(int x1,int y1,int x2,int y2);
	mlSize  GetSize();
	int  Render();
	int  SetEventCall(UiCall callback);
	int  CallUser(Event tevent);
	int  SetTimmer(int time);
	int  KillTimmer(int timmer);
private:
	int StartGL();
	HWND              hWnd;
	HDC               hdc;
	HGLRC             glrc;
	unsigned char     alpha;
	unsigned char     bgcolor[3];
	unsigned char     keycolor[3];
	unsigned int      width;
	unsigned int      height;
	UiCall            EventCall;
};
}
#endif