

#ifndef __THREADWIN_H__
#define __THREADWIN_H__

#include <windows.h>    // Required inclusion on windows

// Define a structure keeping all application data on per window
// basis.
typedef struct _MouseStateRec MouseState;
typedef struct _WindowDataRec WindowData;

struct _MouseStateRec
{
	int x;
	int y;
	bool lbdown;
	bool rbdown;
	bool mbdown;
};
struct _WindowDataRec
{        
	// Initialized data
	void	(*InitOpenGL)(WindowData *pwdata);
	void	(*DrawScene)(WindowData *pwdata);
	void	(*KeyDown)(unsigned int key, WindowData *pwdata);

	int	display;
	char	*windowTitle;

	int	xpos;
	int	ypos;
	int	width;    // Size of client region
	int	height;

	bool	bFullscreen;
	bool	bMainwindow;

	int	windowStyle;
	int	pfdFlags; // Flags for selecting pixel format descriptor


	int	texid;            // Texture id per window
	int	spin;

	HWND	hWnd;     // Main window handle.
	HDC	hDC;      // Device context
	HGLRC	hRC;      // OpenGL context

};

extern MouseState mouse;

extern WindowData wdata[];
extern int winCount;

#endif // __THREADWIN_H__