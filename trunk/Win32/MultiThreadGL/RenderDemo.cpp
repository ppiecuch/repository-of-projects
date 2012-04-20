
#include "ThreadWin.h"
#include <GL/gl.h>      // OpenGL definitions


extern void InitializeOpenGL (WindowData *pwdata);
extern void InitializeOpenGL2(WindowData *pwdata);
extern void DrawScene (WindowData *pwdata);
extern void DrawScene2(WindowData *pwdata);
extern void KeyDownScene(unsigned int key, WindowData* pwdata);

WindowData wdata[] = {
	{
		InitializeOpenGL
		,DrawScene
		,KeyDownScene
		,0
		,"MS OpenGL"
		,10,  10, 640, 480
		,false
		,true
		,//WS_OVERLAPPEDWINDOW |   // Window style 
		//WS_CLIPCHILDREN |       // Needed for OpenGL
		//WS_CLIPSIBLINGS |        // Needed for OpenGL
		WS_POPUP
		//| WS_TILEDWINDOW
		,PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_GENERIC_FORMAT
	},

	{
		InitializeOpenGL2
		,DrawScene2
		,KeyDownScene
		,0
		,"ICD OpenGL"
		, 50, 250, 640, 480
		,false
		,false
 		,//WS_OVERLAPPEDWINDOW |   // Window style 
 		//WS_CLIPCHILDREN |       // Needed for OpenGL
 		//WS_CLIPSIBLINGS |        // Needed for OpenGL
		WS_POPUP |
		WS_TILEDWINDOW
		,PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER
	},    
};

int winCount = 2;

void InitializeOpenGL (WindowData *pwdata)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor (0.2, 0.5, 0.0, 1.0); 
}

void KeyDownScene(unsigned int key, WindowData* pwdata)
{
	switch (key)
	{
	case 32:
		pwdata->spin++;
		PostMessage(pwdata->hWnd, WM_PAINT, 0, 0);
		if (pwdata->spin >= 360) {
			pwdata->spin = 0;
		}

		break;

	case VK_DOWN:
		break;
	default:
		break;

	}
}

void DrawScene(WindowData *pwdata)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor (0.2, 0.5, 0.0, 1.0);

	glPushMatrix();

	glRotatef (pwdata->spin, 0.0, 0.0, 1.0);

	glBegin( GL_POLYGON );
	glColor3f (1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f (0.0, 1.0, 0.0); 
	glVertex3f(0.5, 0.0, 0.0);

	glColor3f (0.0, 0.0, 1.0); 
	glVertex3f(0.5, 0.5, 0.0);          
	glEnd();

	glPopMatrix();
}

void InitializeOpenGL2(WindowData *pwdata)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor (0.5, 0.0, 0.2, 1.0);
}

void DrawScene2(WindowData *pwdata)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor (0.5, 0.0, 0.2, 1.0);          // BLACK

	glPushMatrix();

	glRotatef (pwdata->spin, 0.0, 0.0, 1.0);

	glBegin( GL_POLYGON );
	glColor3f (1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f (0.0, 1.0, 0.0); 
	glVertex3f(0.5, 0.0, 0.0);

	glColor3f (0.0, 0.0, 1.0); 
	glVertex3f(0.5, 0.5, 0.0);          
	glEnd();

	glPopMatrix();
}

