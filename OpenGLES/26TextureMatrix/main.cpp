#include <GL/glew.h>
#include <GL/glut.h>
#include <windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma  comment(lib, "glew32.lib")

#include "Texture.h"
#include "ProjectiveTexture.h"
#include "matrix4x4.h"

#pragma comment(lib, "Texture.lib")

#include <crtdbg.h>
#include <typeinfo>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

enum ENUM_MODE{
	ENUM_MODE_NONE=0,
	ENUM_MODE_ROTATE
};


static float rotx, roty;
static int ox = -1, oy = -1;
static ENUM_MODE mode=ENUM_MODE_NONE;

void rotate(const int x, const int y) {
	rotx += x-ox;
	if (rotx > 360.) rotx -= 360.;
	else if (rotx < -360.) rotx += 360.;
	roty += y-oy;
	if (roty > 360.) roty -= 360.;
	else if (roty < -360.) roty += 360.;
	ox = x; oy = y;
	glutPostRedisplay();
}

CTexture* pTexture=NULL;

GLdouble theta = 0;//旋转角度
Matrix4x4f ObjectTransform, TextureTransform;
float MVmatrix[16];


void display(void) { 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	//glLoadIdentity();
	glTranslatef(0.000f,-0.0001f,0);
	//glRotatef(0.1f, 0.0, 1.0, 0.0);
	//glScalef(1.001f,1.001f,1.001f);
	/*glTranslatef(0.5f,0.5f,0.5f); //Bias
	glScalef(0.5f,0.5f,0.5f);		//Scale(将系数高大，得到的投影贴花反而变小，原因待析)
	glOrtho(-0.835,0.835,-0.835,0.835,0.1,0.2); //MV for light map
	gluLookAt(1,0,1,-1,0,-1,0,1,0);*/
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-10);
	glRotatef(rotx, 0.0, 1.0, 0.0);
	glutSolidTeapot(2.0);*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-10);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f,0.1f);
		glVertex3f(-1,-2,0);
		glTexCoord2f(1.0f,0.2f);
		glVertex3f(1,-2,0);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1,2,0);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1,2,0);
	glEnd();

	glutSwapBuffers();
}
void init()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.1, 0.2, 0.3, 1); 
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5f,0.5,-0.5f,0.5f,1.0f,15.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,640,480);
	gluLookAt(0.0,0.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glEnable(GL_TEXTURE_2D);
	pTexture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\26TextureMatrix\\waterfall3.png");
	pTexture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
void resize(int width,int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(double)width/(double)height,0.1f,100.0f);
}
void idle()
{
	//glutPostRedisplay 标记当前窗口需要重新绘制。通过glutMainLoop下一次循环时，窗口显示将被回调以重新显示窗口的正常面板。
	//多次调用glutPostRedisplay，在下一个显示回调只产生单一的重新显示回调。 
	glutPostRedisplay();
}
void key(unsigned char key, int x, int y)
{
}
void motion(int x, int y)
{
	if(mode==ENUM_MODE_ROTATE)
		rotate(x,y);
}
void mouse(int btn, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(btn) 
		{
		case GLUT_LEFT_BUTTON:
			mode=ENUM_MODE_ROTATE;
			ox=x;
			oy=y;
			motion(x,y);
			break;
		}
	}
	else if(state==GLUT_UP)
	{
		mode=ENUM_MODE_NONE;
	}
}


void destroy()
{
	if(pTexture)
		delete pTexture;
}

class Singleton{
private:
	Singleton(){}
	Singleton(const Singleton&){}
	Singleton& operator = (const Singleton&){}
public:
	static Singleton& GetInstance()
	{  
		static Singleton m_instance;
		return m_instance;
	} 

	~Singleton()
	{
		destroy();
	}
};

int main(int argc, char **argv)
{
	EnableMemLeakCheck();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(640,480); 
	(void) glutCreateWindow("GL TextureMatrix");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	Singleton::GetInstance();//用于资源释放

	glutMainLoop();

	return 0;             /* ANSI C requires main to return int. */
}