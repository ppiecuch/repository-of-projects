#include <GL/glut.h>
// in order to get function prototypes from glext.h, define GL_GLEXT_PROTOTYPES before including glext.h
#define GL_GLEXT_PROTOTYPES
#include "glext.h"
#include "glInfo.h"
#include <iostream>
#include <sstream>
#include <string>

#include "teapot.h"
#include "FPSCounter.h"
#include <string.h>
#include <stdio.h>
#pragma comment(lib,"utility.lib")
FPSCounter fpsCounter;
GLdouble angle = 0;//旋转角度

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TEXTURE_WIDTH = 256;
const int TEXTURE_HEIGHT = 256;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode = 0;
GLuint fboId;                       // ID of FBO
GLuint textureId;                   // ID of texture
GLuint rboId;                       // ID of Renderbuffer object
bool fboSupported;
bool fboUsed;

using namespace std;

// function pointers for FBO extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
// Framebuffer object
PFNGLGENFRAMEBUFFERSEXTPROC                     pglGenFramebuffersEXT = 0;                      // FBO name generation procedure
PFNGLDELETEFRAMEBUFFERSEXTPROC                  pglDeleteFramebuffersEXT = 0;                   // FBO deletion procedure
PFNGLBINDFRAMEBUFFEREXTPROC                     pglBindFramebufferEXT = 0;                      // FBO bind procedure
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              pglCheckFramebufferStatusEXT = 0;               // FBO completeness test procedure
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC pglGetFramebufferAttachmentParameterivEXT = 0;  // return various FBO parameters
PFNGLGENERATEMIPMAPEXTPROC                      pglGenerateMipmapEXT = 0;                       // FBO automatic mipmap generation procedure
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                pglFramebufferTexture2DEXT = 0;                 // FBO texdture attachement procedure
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             pglFramebufferRenderbufferEXT = 0;              // FBO renderbuffer attachement procedure
// Renderbuffer object
PFNGLGENRENDERBUFFERSEXTPROC                    pglGenRenderbuffersEXT = 0;                     // renderbuffer generation procedure
PFNGLDELETERENDERBUFFERSEXTPROC                 pglDeleteRenderbuffersEXT = 0;                  // renderbuffer deletion procedure
PFNGLBINDRENDERBUFFEREXTPROC                    pglBindRenderbufferEXT = 0;                     // renderbuffer bind procedure
PFNGLRENDERBUFFERSTORAGEEXTPROC                 pglRenderbufferStorageEXT = 0;                  // renderbuffer memory allocation procedure
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          pglGetRenderbufferParameterivEXT = 0;           // return various renderbuffer parameters
PFNGLISRENDERBUFFEREXTPROC                      pglIsRenderbufferEXT = 0;                       // determine renderbuffer object type

#define glGenFramebuffersEXT                        pglGenFramebuffersEXT
#define glDeleteFramebuffersEXT                     pglDeleteFramebuffersEXT
#define glBindFramebufferEXT                        pglBindFramebufferEXT
#define glCheckFramebufferStatusEXT                 pglCheckFramebufferStatusEXT
#define glGetFramebufferAttachmentParameterivEXT    pglGetFramebufferAttachmentParameterivEXT
#define glGenerateMipmapEXT                         pglGenerateMipmapEXT
#define glFramebufferTexture2DEXT                   pglFramebufferTexture2DEXT
#define glFramebufferRenderbufferEXT                pglFramebufferRenderbufferEXT

#define glGenRenderbuffersEXT                       pglGenRenderbuffersEXT
#define glDeleteRenderbuffersEXT                    pglDeleteRenderbuffersEXT
#define glBindRenderbufferEXT                       pglBindRenderbufferEXT
#define glRenderbufferStorageEXT                    pglRenderbufferStorageEXT
#define glGetRenderbufferParameterivEXT             pglGetRenderbufferParameterivEXT
#define glIsRenderbufferEXT                         pglIsRenderbufferEXT

#endif

void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
bool checkFramebufferStatus();
void printFramebufferInfo();

void printFPS(){
	//Update frames per second counter
	fpsCounter.update();

	//Print fps
	static char fpsString[32];
	sprintf_s(fpsString, "FPS:%.2f", fpsCounter.getFPS());

	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//Print text
	glRasterPos2f(-1.0f, 0.9f);
	for(unsigned int i=0; i<strlen(fpsString); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
///////////////////////////////////////////////////////////////////////////////
// draw a textured cube
///////////////////////////////////////////////////////////////////////////////
void draw()
{
	glBindTexture(GL_TEXTURE_2D, textureId);

	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);

	// face v0-v1-v2-v3
	glNormal3f(0,0,1);
	glTexCoord2f(1, 1);  glVertex3f(1,1,1);
	glTexCoord2f(0, 1);  glVertex3f(-1,1,1);
	glTexCoord2f(0, 0);  glVertex3f(-1,-1,1);
	glTexCoord2f(1, 0);  glVertex3f(1,-1,1);

	// face v0-v3-v4-v5
	glNormal3f(1,0,0);
	glTexCoord2f(0, 1);  glVertex3f(1,1,1);
	glTexCoord2f(0, 0);  glVertex3f(1,-1,1);
	glTexCoord2f(1, 0);  glVertex3f(1,-1,-1);
	glTexCoord2f(1, 1);  glVertex3f(1,1,-1);

	// face v0-v5-v6-v1
	glNormal3f(0,1,0);
	glTexCoord2f(1, 0);  glVertex3f(1,1,1);
	glTexCoord2f(1, 1);  glVertex3f(1,1,-1);
	glTexCoord2f(0, 1);  glVertex3f(-1,1,-1);
	glTexCoord2f(0, 0);  glVertex3f(-1,1,1);

	// face  v1-v6-v7-v2
	glNormal3f(-1,0,0);
	glTexCoord2f(1, 1);  glVertex3f(-1,1,1);
	glTexCoord2f(0, 1);  glVertex3f(-1,1,-1);
	glTexCoord2f(0, 0);  glVertex3f(-1,-1,-1);
	glTexCoord2f(1, 0);  glVertex3f(-1,-1,1);

	// face v7-v4-v3-v2
	glNormal3f(0,-1,0);
	glTexCoord2f(0, 0);  glVertex3f(-1,-1,-1);
	glTexCoord2f(1, 0);  glVertex3f(1,-1,-1);
	glTexCoord2f(1, 1);  glVertex3f(1,-1,1);
	glTexCoord2f(0, 1);  glVertex3f(-1,-1,1);

	// face v4-v7-v6-v5
	glNormal3f(0,0,-1);
	glTexCoord2f(0, 0);  glVertex3f(1,-1,-1);
	glTexCoord2f(1, 0);  glVertex3f(-1,-1,-1);
	glTexCoord2f(1, 1);  glVertex3f(-1,1,-1);
	glTexCoord2f(0, 1);  glVertex3f(1,1,-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}
int a=0;
void display(void) { 
	// adjust viewport and projection matrix to texture dimension
	//glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glEnable(GL_SCISSOR_TEST);
	if(++a%200==0)
	{
		glScissor(TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2, TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2);
		glViewport(TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2, TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2);
	}
	else
	{
		glScissor(0,0,TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2);
		glViewport(0, 0, TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(TEXTURE_WIDTH)/TEXTURE_HEIGHT, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	// with FBO
	// render directly to a texture
	if(fboUsed)
	{
		// set the rendering destination to FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

		// clear buffer
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw a rotating teapot
		glPushMatrix();
		glRotatef(angle*0.5f, 1, 0, 0);
		glRotatef(angle, 0, 1, 0);
		glRotatef(angle*0.7f, 0, 0, 1);
		glTranslatef(0, -1.575f, 0);
		drawTeapot();
		glPopMatrix();

		// back to normal window-system-provided framebuffer
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind

		// trigger mipmaps generation explicitly
		// NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
		// triggers mipmap generation automatically. However, the texture attached
		// onto a FBO should generate mipmaps manually via glGenerateMipmapEXT().
		glBindTexture(GL_TEXTURE_2D, textureId);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// without FBO
	// render to the backbuffer and copy the backbuffer to a texture
	else
	{
		// clear buffer
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_ALPHA_TEST);

		// draw a rotating teapot
		glPushMatrix();
		glRotatef(angle*0.5f, 1, 0, 0);
		glRotatef(angle, 0, 1, 0);
		glRotatef(angle*0.7f, 0, 0, 1);
		glTranslatef(0, -1.575f, 0);
		drawTeapot();
		glPopMatrix();

		// copy the framebuffer pixels to a texture
		glBindTexture(GL_TEXTURE_2D, textureId);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		glBindTexture(GL_TEXTURE_2D, 0);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glDisable(GL_ALPHA_TEST);

		glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
	}

	// back to normal viewport and projection matrix
	//glViewport(-96, -96, SCREEN_WIDTH-96, SCREEN_HEIGHT-96);
	glDisable(GL_SCISSOR_TEST);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(SCREEN_WIDTH)/SCREEN_HEIGHT, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	// clear framebuffer
	glClearColor(0.5f,0.5f,0.5f,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();

	// tramsform camera
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);   // pitch
	glRotatef(cameraAngleY, 0, 1, 0);   // heading

	// draw a cube with the dynamic texture
	draw();

	glPopMatrix();

	printFPS();

	angle+=0.5f;

	glutSwapBuffers();
}
void init()
{
	

	mouseLeftDown = mouseRightDown = false;
	cameraAngleX = cameraAngleY = 45;
	cameraDistance = 0;

	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

	// enable /disable features
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);                   // background color
	glClearStencil(0);                          // clear stencil buffer
	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);

	initLights();
	setCamera(0, 0, 6, 0, 0, 0);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	bool ScissorTest=glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
	printf("ScissorTest:%d\n",ScissorTest);
	glEnable(GL_SCISSOR_TEST);
	ScissorTest=glIsEnabled(GL_SCISSOR_TEST)==GL_TRUE;
	printf("ScissorTest:%d\n",ScissorTest);
	//glScissor(16,16,512,256);

	// get OpenGL info
	glInfo glInfo;
	glInfo.getInfo();
	glInfo.printSelf();

#ifdef _WIN32
	// check if FBO is supported by your video card
	if(glInfo.isExtensionSupported("GL_EXT_framebuffer_object"))
	{
		// get pointers to GL functions
		glGenFramebuffersEXT                     = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glDeleteFramebuffersEXT                  = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glBindFramebufferEXT                     = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glCheckFramebufferStatusEXT              = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT                      = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
		glFramebufferTexture2DEXT                = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferRenderbufferEXT             = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGenRenderbuffersEXT                    = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glDeleteRenderbuffersEXT                 = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glBindRenderbufferEXT                    = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glRenderbufferStorageEXT                 = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT          = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsRenderbufferEXT                      = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");

		// check once again FBO extension
		if(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
			glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT && glFramebufferRenderbufferEXT &&
			glGenRenderbuffersEXT && glDeleteRenderbuffersEXT && glBindRenderbufferEXT && glRenderbufferStorageEXT &&
			glGetRenderbufferParameterivEXT && glIsRenderbufferEXT)
		{
			fboSupported = fboUsed = true;
			cout << "Video card supports GL_EXT_framebuffer_object." << endl;
		}
		else
		{
			fboSupported = fboUsed = false;
			cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
		}
	}

#else // for linux, do not need to get function pointers, it is up-to-date
	if(glInfo.isExtensionSupported("GL_EXT_framebuffer_object"))
	{
		fboSupported = fboUsed = true;
		cout << "Video card supports GL_EXT_framebuffer_object." << endl;
	}
	else
	{
		fboSupported = fboUsed = false;
		cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
	}
#endif

	

	if(fboSupported)
	{
		//创建一个FBO容器
		glGenFramebuffersEXT(1,&fboId);
		//glBindFramebufferEXT可以用于FBO之间进行切换，绑定到名称0将会解除当前绑定的FBO，并把渲染重新定向到窗口的帧缓冲区
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);

		//创建一个RBO对象用于存储深度信息
		//注意：FBO必须挂接一张存放深度信息的图像，否则因为深度测试的缺失会导致输出结果错乱
		//如果你同时需要模板测试，那么FBO还需要挂接一个用于存放模板信息的图像
		glGenRenderbuffersEXT(1,&rboId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,rboId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,TEXTURE_WIDTH,TEXTURE_HEIGHT);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);//为什么需要这个？

		//挂接一张纹理
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,textureId,0);

		//挂接RBO
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,rboId);

		//@ disable color buffer if you don't attach any color buffer image,
		//@ for example, rendering depth buffer only to a texture.
		//@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);

		// check FBO status
		printFramebufferInfo();
		bool status = checkFramebufferStatus();
		if(!status)
			fboUsed = false;

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}
void resize(int width,int height)
{
}
void idle()
{
	//glutPostRedisplay 标记当前窗口需要重新绘制。通过glutMainLoop下一次循环时，窗口显示将被回调以重新显示窗口的正常面板。
	//多次调用glutPostRedisplay，在下一个显示回调只产生单一的重新显示回调。 
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: // ESCAPE
		exit(0);
		break;

	case ' ':
		if(fboSupported)
			fboUsed = !fboUsed;
		cout << "FBO mode: " << (fboUsed ? "on" : "off") << endl;
		break;

	case 'd': // switch rendering modes (fill -> wire -> point)
	case 'D':
		drawMode = ++drawMode % 3;
		if(drawMode == 0)        // fill mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
		}
		else if(drawMode == 1)  // wireframe mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		else                    // point mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		break;

	default:
		;
	}
}
void mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if(state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if(state == GLUT_UP)
			mouseRightDown = false;
	}
}
void motion(int x, int y)
{
	if(mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if(mouseRightDown)
	{
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}
}

void destroy()
{

	//glDeleteTextures(1, &textureId);

	// clean up FBO, RBO
	if(fboSupported)
	{
		glDeleteFramebuffersEXT(1, &fboId);
		glDeleteRenderbuffersEXT(1, &rboId);
	}

	printf("destroy");
}
int main(int argc, char **argv)
{
	atexit(destroy);
	//初始化GLUT
	//Void glutInit(int*argc,char**argv)； 
	//参数： 
	//Argc：一个指针，指向从main（）函数传递过来的没更改的argc变量。 
	//Argv：一个指针，指向从main（）函数传递过来的没更改的argv变量。
	glutInit(&argc, argv);
	//定义显示方式。 
	//Void glutInitDisplayMode(unsighed int mode) 
	//参数： 
	//Mode――可以指定下列显示模式 
	//Mode参数是一个GLUT库里预定义的可能的布尔组合。你使用mode去指定颜色模式，数量和缓冲区类型。 
	//指定颜色模式的预定义常量有： 
	//1：GLUT_RGBA或者GLUT_RGB。指定一个RGBA窗口，这是一个默认的颜色模式。 
	//2：GLUT_INDEX。指定颜色索引模式。 
	//这个显示模式还允许你选择单缓冲区或双缓冲区窗口。 
	//1：GLUT_SINGLE.单缓冲区窗口。 
	//2：GLUT_BUFFER.双缓冲区窗口，这是产生流畅动画必须选的。 
	//还可以指定更多，如果你想指定一组特殊的缓冲的话，用下面的变量： 
	//1：GLUT_ACCUM.累积缓冲区。 
	//2：GLUT_STENCIL.模板缓冲区。 
	//3：GLUT_DEPTH.深度缓冲区。 
	//用“或“（|）操作符来建立你想要的显示模式。 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);
	//确定窗口位置（它默认的是屏幕左上角），我们使用函数glutInitWindowPosition（）。 
	//Void glutInitWindowPositon(int x,int y); 
	//参数： 
	//X: 距离屏幕左边的像素数。－1是默认值，意思就是由窗口管理程序决定窗口出现在哪里。如果不使用默认值，那你就自己设置一个值。 
	//Y：距离屏幕上边的像素数。和X一样。 
	//注意，参数仅仅是对窗口管理程序的一个建议。尽管你精心的设置了窗口位置，window返回的可能是不同的位置。如果你设置了，一般会得到你想要的结果。
	glutInitWindowPosition(100,100); 
	//设置窗口大小，使用函数glutInitWindowSize（）。 
	//Void glutInitWindowSize(int width,int height); 
	//参数： 
	//Width：窗口的宽度。 
	//Height：窗口的高度。 
	//同样width，height也只是一个参考数字。避免使用负数。 
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT); 
	//调用函数glutCreateWindow()来创建窗口
	//Int glutCreateWindow(char* title); 
	//参数： 
	//Title：设置窗口的标题。 
	//glutCreateWindow（）的返回值是一个窗口标识符。
	(void) glutCreateWindow("GL Framework");


	//告诉GLUT使用我们上面的函数来进行渲染。这个叫寄存回调。。让我们告诉GLUT这个函数renderScene应该被使用。
	//当需要重画的时候GLUT有一个只传递一个函数名称参数的函数（以函数名为形参的函数）就会被调用。 
	//void glutDisplayFunc(void (*func)(void)); 
	//参数： 
	//func: 当窗口需要被重绘是调用的函数的名称。注意使用NULL作为实参是错误的。
	glutDisplayFunc(display);
	//在glutIdleFunc(void (*func)(void))回调函数中指定一个函数，如果不存在其他尚未完成的事件（例如，当事件循环处于空闲的时候），就执行这个函数。
	//这个回调函数接受一个函数指针作为它的唯一参数。如果向它传递NULL(0),就相当于禁用这个函数。
	glutIdleFunc(idle);
	//窗口发生变化时,视窗需要重新计算.
	//void glutReshapeFunc(void (*func)(int width, int height)); 
	//参数: 
	//func -- The name of the function that will be responsible forsetting the correct perspective when the window changes size. 
	glutReshapeFunc(resize);
	//void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
	// key 为键盘按键的ASCII码
	glutKeyboardFunc(key);
	//检测鼠标Clicks
	//和键盘处理一样，GLUT为你的注册函数（也就是处理鼠标clicks事件的函数）提供了一个方法。函数glutMouseFunc,这个函数一般在程序初始化阶段被调用。函数原型如下：
	//void glutMouseFunc(void(*func)(int button,int state,int x,int y));
	//参数：
	//func：处理鼠标click事件的函数的函数名。
	//从上面可以看到到，处理鼠标click事件的函数，一定有4个参数。
	//第一个参数表明哪个鼠标键被按下或松开，这个变量可以是下面的三个值中的一个：
	//GLUT_LEFT_BUTTON
	//GLUT_MIDDLE_BUTTON
	//GLUT_RIGHT_BUTTON
	//第二个参数表明，函数被调用发生时，鼠标的状态，也就是是被按下，或松开，可能取值如下：
	//GLUT_DOWN
	//GLUT_UP
	//当函数被调用时，state的值是GLUT_DOWN，那么程序可能会假定将会有个GLUT_UP事件，甚至鼠标移动到窗口外面，也如此。
	//然而，如果程序调用glutMouseFunc传递NULL作为参数，那么GLUT将不会改变鼠标的状态。
	//剩下的两个参数（x,y）提供了鼠标当前的窗口坐标（以左上角为原点）。
	glutMouseFunc(mouse);
	//检测动作（motion）
	//GLUT提供鼠标motion检测能力。有两种GLUT处理的motion：active motion和passive motion。
	//Active motion是指鼠标移动并且有一个鼠标键被按下。（拖动鼠标）
	//Passive motion是指当鼠标移动时，并有没鼠标键按下。（移动鼠标）
	//如果一个程序正在追踪鼠标，那么鼠标移动期间，没一帧将产生一个结果。
	//和以前一样，你必须注册将处理鼠标事件的函数（定义函数）。GLUT让我们可以指定两个不同的函数，一个追踪passive motion，另一个追踪active motion
	//它们的函数原型，如下：
	//void glutMotionFunc(void(*func)(int x,int y));
	//void glutPassiveMotionFunc(void (*func)(int x,int y));
	//参数：
	//Func：处理各自类型motion的函数名。
	//处理motion的参数函数的参数（x,y）是鼠标在窗口的坐标。以左上角为原点。
	glutMotionFunc(motion);

	init();

	//检测鼠标进入或离开窗口
	//GLUT还能检测鼠标鼠标离开，进入窗口区域。一个回调函数可以被定义去处理这两个事件。GLUT里，调用这个函数的是glutEntryFunc,函数原型如下：
	//void glutEntryFunc(void(*func)（int state）);
	//参数：
	//Func：处理这些事件的函数名。
	//上面函数的参数中，state有两个值：
	//GLUT_LEFT      鼠标离开窗口
	//GLUT_ENTERED  鼠标进入窗口
	//表明，是离开，还是进入窗口。
	//glutEntryFunc(entry);
	//glutVisibilityFunc(vis);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Toggle showing projection", 's');
	//glutAddMenuEntry("Switch texture", 't');
	//glutAddMenuEntry("Switch object", 'o');
	//glutAddMenuEntry("Toggle filtering", 'l');
	//glutAddMenuEntry("Quit", 666);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
	//如果用定时器的话，初始的时候注册一个定时器的回调函数，原型是
	//glutTimerFunc(unsigned int millis, void (*func)(int value), int value);
	//这样使用glutTimerFunc(毫秒数, 回调函数指针, 区别值);
	//写自己的回调函数
	//void OnTimer(int value);
	//用value区分是哪个定时器
	//在函数里改变和位置有关的变量，然后调用glutPostRedisplay();用来重绘
	//最后再次调用glutTimerFunc，因为glut的定时器是调用一次才产生一次定时，所以如果要持续产生定时的话，在定时函数末尾再次调用glutTimerFunc


	//告诉GLUT我们准备进入应用程序事件处理循环。GLUT提供了一个函数让程序进入一个永不结束的循环。一直等待处理下一个事件。函数是glutMainLoop（）。 
	//glutMainLoop()一旦进入循环就再也不返回
	//glutMainLoop()  阻断了系统的正常流程，在关闭glutCreateWindow()创建的窗口后，glutMainLoop()直接用exit(0)退出程序，而不会继续执行glutMainLoop()后的语句，
	//这就使一些类的释放及资源回收工作无法进行，从而造成内存泄漏。纠正此问题，有3个办法：
	//1、使用glutLeaveMainLoop()代替glutMainLoop()。
	//2、处理代码中exit(0)的部分。 
	//3、在glutMainLoop之前先设置： 
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	//然后再用glutMainLoop();则 glutMainLoop()会在退出后，继续执行其后的代码。
	//4、创建单例类对象，并在其销毁函数中调用你的资源释放处理
	//使用以上方法，可以有效处理glutMainLoop()造成的内存泄漏！
	//void glutMainLoop(void) 
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}


///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
	GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
	GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = {0, 0, 20, 1}; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}


///////////////////////////////////////////////////////////////////////////////
// convert OpenGL internal format enum to string
///////////////////////////////////////////////////////////////////////////////
std::string convertInternalFormatToString(GLenum format)
{
	std::string formatName;

	switch(format)
	{
	case GL_STENCIL_INDEX:
		formatName = "GL_STENCIL_INDEX";
		break;
	case GL_DEPTH_COMPONENT:
		formatName = "GL_DEPTH_COMPONENT";
		break;
	case GL_ALPHA:
		formatName = "GL_ALPHA";
		break;
	case GL_RGB:
		formatName = "GL_RGB";
		break;
	case GL_RGBA:
		formatName = "GL_RGBA";
		break;
	case GL_LUMINANCE:
		formatName = "GL_LUMINANCE";
		break;
	case GL_LUMINANCE_ALPHA:
		formatName = "GL_LUMINANCE_ALPHA";
		break;
	case GL_ALPHA4:
		formatName = "GL_ALPHA4";
		break;
	case GL_ALPHA8:
		formatName = "GL_ALPHA8";
		break;
	case GL_ALPHA12:
		formatName = "GL_ALPHA12";
		break;
	case GL_ALPHA16:
		formatName = "GL_ALPHA16";
		break;
	case GL_LUMINANCE4:
		formatName = "GL_LUMINANCE4";
		break;
	case GL_LUMINANCE8:
		formatName = "GL_LUMINANCE8";
		break;
	case GL_LUMINANCE12:
		formatName = "GL_LUMINANCE12";
		break;
	case GL_LUMINANCE16:
		formatName = "GL_LUMINANCE16";
		break;
	case GL_LUMINANCE4_ALPHA4:
		formatName = "GL_LUMINANCE4_ALPHA4";
		break;
	case GL_LUMINANCE6_ALPHA2:
		formatName = "GL_LUMINANCE6_ALPHA2";
		break;
	case GL_LUMINANCE8_ALPHA8:
		formatName = "GL_LUMINANCE8_ALPHA8";
		break;
	case GL_LUMINANCE12_ALPHA4:
		formatName = "GL_LUMINANCE12_ALPHA4";
		break;
	case GL_LUMINANCE12_ALPHA12:
		formatName = "GL_LUMINANCE12_ALPHA12";
		break;
	case GL_LUMINANCE16_ALPHA16:
		formatName = "GL_LUMINANCE16_ALPHA16";
		break;
	case GL_INTENSITY:
		formatName = "GL_INTENSITY";
		break;
	case GL_INTENSITY4:
		formatName = "GL_INTENSITY4";
		break;
	case GL_INTENSITY8:
		formatName = "GL_INTENSITY8";
		break;
	case GL_INTENSITY12:
		formatName = "GL_INTENSITY12";
		break;
	case GL_INTENSITY16:
		formatName = "GL_INTENSITY16";
		break;
	case GL_R3_G3_B2:
		formatName = "GL_R3_G3_B2";
		break;
	case GL_RGB4:
		formatName = "GL_RGB4";
		break;
	case GL_RGB5:
		formatName = "GL_RGB4";
		break;
	case GL_RGB8:
		formatName = "GL_RGB8";
		break;
	case GL_RGB10:
		formatName = "GL_RGB10";
		break;
	case GL_RGB12:
		formatName = "GL_RGB12";
		break;
	case GL_RGB16:
		formatName = "GL_RGB16";
		break;
	case GL_RGBA2:
		formatName = "GL_RGBA2";
		break;
	case GL_RGBA4:
		formatName = "GL_RGBA4";
		break;
	case GL_RGB5_A1:
		formatName = "GL_RGB5_A1";
		break;
	case GL_RGBA8:
		formatName = "GL_RGBA8";
		break;
	case GL_RGB10_A2:
		formatName = "GL_RGB10_A2";
		break;
	case GL_RGBA12:
		formatName = "GL_RGBA12";
		break;
	case GL_RGBA16:
		formatName = "GL_RGBA16";
		break;
	default:
		formatName = "Unknown Format";
	}

	return formatName;
}

///////////////////////////////////////////////////////////////////////////////
// return texture parameters as string using glGetTexLevelParameteriv()
///////////////////////////////////////////////////////////////////////////////
std::string getTextureParameters(GLuint id)
{
	if(glIsTexture(id) == GL_FALSE)
		return "Not texture object";

	int width, height, format;
	std::string formatName;
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
	glBindTexture(GL_TEXTURE_2D, 0);

	formatName = convertInternalFormatToString(format);

	std::stringstream ss;
	ss << width << "x" << height << ", " << formatName;
	return ss.str();
}



///////////////////////////////////////////////////////////////////////////////
// return renderbuffer parameters as string using glGetRenderbufferParameterivEXT
///////////////////////////////////////////////////////////////////////////////
std::string getRenderbufferParameters(GLuint id)
{
	if(glIsRenderbufferEXT(id) == GL_FALSE)
		return "Not Renderbuffer object";

	int width, height, format;
	std::string formatName;
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
	glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &width);    // get renderbuffer width
	glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &height);  // get renderbuffer height
	glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_INTERNAL_FORMAT_EXT, &format); // get renderbuffer internal format
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	formatName = convertInternalFormatToString(format);

	std::stringstream ss;
	ss << width << "x" << height << ", " << formatName;
	return ss.str();
}


///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool checkFramebufferStatus()
{
	// check FBO status
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		std::cout << "Framebuffer complete." << std::endl;
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
		return false;

	default:
		std::cout << "[ERROR] Unknow error." << std::endl;
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////
// print out the FBO infos
///////////////////////////////////////////////////////////////////////////////
void printFramebufferInfo()
{
	cout << "\n***** FBO STATUS *****\n";

	// print max # of colorbuffers supported by FBO
	int colorBufferCount = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &colorBufferCount);
	cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << endl;

	int objectType;
	int objectId;

	// print info of the colorbuffer attachable image
	for(int i = 0; i < colorBufferCount; ++i)
	{
		glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
			GL_COLOR_ATTACHMENT0_EXT+i,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
			&objectType);
		if(objectType != GL_NONE)
		{
			glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
				GL_COLOR_ATTACHMENT0_EXT+i,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
				&objectId);

			std::string formatName;

			cout << "Color Attachment " << i << ": ";
			if(objectType == GL_TEXTURE)
				cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
			else if(objectType == GL_RENDERBUFFER_EXT)
				cout << "GL_RENDERBUFFER_EXT, " << getRenderbufferParameters(objectId) << endl;
		}
	}

	// print info of the depthbuffer attachable image
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
			GL_DEPTH_ATTACHMENT_EXT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
			&objectId);

		cout << "Depth Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
			break;
		case GL_RENDERBUFFER_EXT:
			cout << "GL_RENDERBUFFER_EXT, " << getRenderbufferParameters(objectId) << endl;
			break;
		}
	}

	// print info of the stencilbuffer attachable image
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
		GL_STENCIL_ATTACHMENT_EXT,
		GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT,
		&objectType);
	if(objectType != GL_NONE)
	{
		glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT,
			GL_STENCIL_ATTACHMENT_EXT,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT,
			&objectId);

		cout << "Stencil Attachment: ";
		switch(objectType)
		{
		case GL_TEXTURE:
			cout << "GL_TEXTURE, " << getTextureParameters(objectId) << endl;
			break;
		case GL_RENDERBUFFER_EXT:
			cout << "GL_RENDERBUFFER_EXT, " << getRenderbufferParameters(objectId) << endl;
			break;
		}
	}

	cout << endl;
}