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
GLdouble angle = 0;//��ת�Ƕ�

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
		//����һ��FBO����
		glGenFramebuffersEXT(1,&fboId);
		//glBindFramebufferEXT��������FBO֮������л����󶨵�����0��������ǰ�󶨵�FBO��������Ⱦ���¶��򵽴��ڵ�֡������
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);

		//����һ��RBO�������ڴ洢�����Ϣ
		//ע�⣺FBO����ҽ�һ�Ŵ�������Ϣ��ͼ�񣬷�����Ϊ��Ȳ��Ե�ȱʧ�ᵼ������������
		//�����ͬʱ��Ҫģ����ԣ���ôFBO����Ҫ�ҽ�һ�����ڴ��ģ����Ϣ��ͼ��
		glGenRenderbuffersEXT(1,&rboId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,rboId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,TEXTURE_WIDTH,TEXTURE_HEIGHT);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);//Ϊʲô��Ҫ�����

		//�ҽ�һ������
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,textureId,0);

		//�ҽ�RBO
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
	//glutPostRedisplay ��ǵ�ǰ������Ҫ���»��ơ�ͨ��glutMainLoop��һ��ѭ��ʱ��������ʾ�����ص���������ʾ���ڵ�������塣
	//��ε���glutPostRedisplay������һ����ʾ�ص�ֻ������һ��������ʾ�ص��� 
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
	//��ʼ��GLUT
	//Void glutInit(int*argc,char**argv)�� 
	//������ 
	//Argc��һ��ָ�룬ָ���main�����������ݹ�����û���ĵ�argc������ 
	//Argv��һ��ָ�룬ָ���main�����������ݹ�����û���ĵ�argv������
	glutInit(&argc, argv);
	//������ʾ��ʽ�� 
	//Void glutInitDisplayMode(unsighed int mode) 
	//������ 
	//Mode�D�D����ָ��������ʾģʽ 
	//Mode������һ��GLUT����Ԥ����Ŀ��ܵĲ�����ϡ���ʹ��modeȥָ����ɫģʽ�������ͻ��������͡� 
	//ָ����ɫģʽ��Ԥ���峣���У� 
	//1��GLUT_RGBA����GLUT_RGB��ָ��һ��RGBA���ڣ�����һ��Ĭ�ϵ���ɫģʽ�� 
	//2��GLUT_INDEX��ָ����ɫ����ģʽ�� 
	//�����ʾģʽ��������ѡ�񵥻�������˫���������ڡ� 
	//1��GLUT_SINGLE.�����������ڡ� 
	//2��GLUT_BUFFER.˫���������ڣ����ǲ���������������ѡ�ġ� 
	//������ָ�����࣬�������ָ��һ������Ļ���Ļ���������ı����� 
	//1��GLUT_ACCUM.�ۻ��������� 
	//2��GLUT_STENCIL.ģ�建������ 
	//3��GLUT_DEPTH.��Ȼ������� 
	//�á��򡰣�|������������������Ҫ����ʾģʽ�� 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);
	//ȷ������λ�ã���Ĭ�ϵ�����Ļ���Ͻǣ�������ʹ�ú���glutInitWindowPosition������ 
	//Void glutInitWindowPositon(int x,int y); 
	//������ 
	//X: ������Ļ��ߵ�����������1��Ĭ��ֵ����˼�����ɴ��ڹ������������ڳ�������������ʹ��Ĭ��ֵ��������Լ�����һ��ֵ�� 
	//Y��������Ļ�ϱߵ�����������Xһ���� 
	//ע�⣬���������ǶԴ��ڹ�������һ�����顣�����㾫�ĵ������˴���λ�ã�window���صĿ����ǲ�ͬ��λ�á�����������ˣ�һ���õ�����Ҫ�Ľ����
	glutInitWindowPosition(100,100); 
	//���ô��ڴ�С��ʹ�ú���glutInitWindowSize������ 
	//Void glutInitWindowSize(int width,int height); 
	//������ 
	//Width�����ڵĿ�ȡ� 
	//Height�����ڵĸ߶ȡ� 
	//ͬ��width��heightҲֻ��һ���ο����֡�����ʹ�ø����� 
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT); 
	//���ú���glutCreateWindow()����������
	//Int glutCreateWindow(char* title); 
	//������ 
	//Title�����ô��ڵı��⡣ 
	//glutCreateWindow�����ķ���ֵ��һ�����ڱ�ʶ����
	(void) glutCreateWindow("GL Framework");


	//����GLUTʹ����������ĺ�����������Ⱦ������мĴ�ص����������Ǹ���GLUT�������renderSceneӦ�ñ�ʹ�á�
	//����Ҫ�ػ���ʱ��GLUT��һ��ֻ����һ���������Ʋ����ĺ������Ժ�����Ϊ�βεĺ������ͻᱻ���á� 
	//void glutDisplayFunc(void (*func)(void)); 
	//������ 
	//func: ��������Ҫ���ػ��ǵ��õĺ��������ơ�ע��ʹ��NULL��Ϊʵ���Ǵ���ġ�
	glutDisplayFunc(display);
	//��glutIdleFunc(void (*func)(void))�ص�������ָ��һ�����������������������δ��ɵ��¼������磬���¼�ѭ�����ڿ��е�ʱ�򣩣���ִ�����������
	//����ص���������һ������ָ����Ϊ����Ψһ�����������������NULL(0),���൱�ڽ������������
	glutIdleFunc(idle);
	//���ڷ����仯ʱ,�Ӵ���Ҫ���¼���.
	//void glutReshapeFunc(void (*func)(int width, int height)); 
	//����: 
	//func -- The name of the function that will be responsible forsetting the correct perspective when the window changes size. 
	glutReshapeFunc(resize);
	//void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
	// key Ϊ���̰�����ASCII��
	glutKeyboardFunc(key);
	//������Clicks
	//�ͼ��̴���һ����GLUTΪ���ע�ắ����Ҳ���Ǵ������clicks�¼��ĺ������ṩ��һ������������glutMouseFunc,�������һ���ڳ����ʼ���׶α����á�����ԭ�����£�
	//void glutMouseFunc(void(*func)(int button,int state,int x,int y));
	//������
	//func���������click�¼��ĺ����ĺ�������
	//��������Կ��������������click�¼��ĺ�����һ����4��������
	//��һ�����������ĸ����������»��ɿ�������������������������ֵ�е�һ����
	//GLUT_LEFT_BUTTON
	//GLUT_MIDDLE_BUTTON
	//GLUT_RIGHT_BUTTON
	//�ڶ����������������������÷���ʱ������״̬��Ҳ�����Ǳ����£����ɿ�������ȡֵ���£�
	//GLUT_DOWN
	//GLUT_UP
	//������������ʱ��state��ֵ��GLUT_DOWN����ô������ܻ�ٶ������и�GLUT_UP�¼�����������ƶ����������棬Ҳ��ˡ�
	//Ȼ��������������glutMouseFunc����NULL��Ϊ��������ôGLUT������ı�����״̬��
	//ʣ�µ�����������x,y���ṩ����굱ǰ�Ĵ������꣨�����Ͻ�Ϊԭ�㣩��
	glutMouseFunc(mouse);
	//��⶯����motion��
	//GLUT�ṩ���motion���������������GLUT�����motion��active motion��passive motion��
	//Active motion��ָ����ƶ�������һ�����������¡����϶���꣩
	//Passive motion��ָ������ƶ�ʱ������û�������¡����ƶ���꣩
	//���һ����������׷����꣬��ô����ƶ��ڼ䣬ûһ֡������һ�������
	//����ǰһ���������ע�Ὣ��������¼��ĺ��������庯������GLUT�����ǿ���ָ��������ͬ�ĺ�����һ��׷��passive motion����һ��׷��active motion
	//���ǵĺ���ԭ�ͣ����£�
	//void glutMotionFunc(void(*func)(int x,int y));
	//void glutPassiveMotionFunc(void (*func)(int x,int y));
	//������
	//Func�������������motion�ĺ�������
	//����motion�Ĳ��������Ĳ�����x,y��������ڴ��ڵ����ꡣ�����Ͻ�Ϊԭ�㡣
	glutMotionFunc(motion);

	init();

	//�����������뿪����
	//GLUT���ܼ���������뿪�����봰������һ���ص��������Ա�����ȥ�����������¼���GLUT����������������glutEntryFunc,����ԭ�����£�
	//void glutEntryFunc(void(*func)��int state��);
	//������
	//Func��������Щ�¼��ĺ�������
	//���溯���Ĳ����У�state������ֵ��
	//GLUT_LEFT      ����뿪����
	//GLUT_ENTERED  �����봰��
	//���������뿪�����ǽ��봰�ڡ�
	//glutEntryFunc(entry);
	//glutVisibilityFunc(vis);
	//glutCreateMenu(menu);
	//glutAddMenuEntry("Toggle showing projection", 's');
	//glutAddMenuEntry("Switch texture", 't');
	//glutAddMenuEntry("Switch object", 'o');
	//glutAddMenuEntry("Toggle filtering", 'l');
	//glutAddMenuEntry("Quit", 666);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
	//����ö�ʱ���Ļ�����ʼ��ʱ��ע��һ����ʱ���Ļص�������ԭ����
	//glutTimerFunc(unsigned int millis, void (*func)(int value), int value);
	//����ʹ��glutTimerFunc(������, �ص�����ָ��, ����ֵ);
	//д�Լ��Ļص�����
	//void OnTimer(int value);
	//��value�������ĸ���ʱ��
	//�ں�����ı��λ���йصı�����Ȼ�����glutPostRedisplay();�����ػ�
	//����ٴε���glutTimerFunc����Ϊglut�Ķ�ʱ���ǵ���һ�βŲ���һ�ζ�ʱ���������Ҫ����������ʱ�Ļ����ڶ�ʱ����ĩβ�ٴε���glutTimerFunc


	//����GLUT����׼������Ӧ�ó����¼�����ѭ����GLUT�ṩ��һ�������ó������һ������������ѭ����һֱ�ȴ�������һ���¼���������glutMainLoop������ 
	//glutMainLoop()һ������ѭ������Ҳ������
	//glutMainLoop()  �����ϵͳ���������̣��ڹر�glutCreateWindow()�����Ĵ��ں�glutMainLoop()ֱ����exit(0)�˳����򣬶��������ִ��glutMainLoop()�����䣬
	//���ʹһЩ����ͷż���Դ���չ����޷����У��Ӷ�����ڴ�й©�����������⣬��3���취��
	//1��ʹ��glutLeaveMainLoop()����glutMainLoop()��
	//2�����������exit(0)�Ĳ��֡� 
	//3����glutMainLoop֮ǰ�����ã� 
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); 
	//Ȼ������glutMainLoop();�� glutMainLoop()�����˳��󣬼���ִ�����Ĵ��롣
	//4��������������󣬲��������ٺ����е��������Դ�ͷŴ���
	//ʹ�����Ϸ�����������Ч����glutMainLoop()��ɵ��ڴ�й©��
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