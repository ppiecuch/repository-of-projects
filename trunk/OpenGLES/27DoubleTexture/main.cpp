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
PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
CTexture* pTexture=NULL;
CTexture* pTexture2=NULL;

GLdouble theta = 0;//旋转角度
Matrix4x4f ObjectTransform, TextureTransform;
float MVmatrix[16];


float alpha=1.0f;
float inc=0.00001f;

float vertices[]={-2,-4,0,2,-4,0,-2,4,0,2,4,0};
float texcoords[]={0,0,1,0,0,0.1f,1,0.1f};
float texcoords2[]={0,0,1,0,0,1,1,1};
unsigned short indices[]={0,1,2,3};
float trans=0;


void display(void) { 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0,vertices);

	glActiveTextureARB(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glTranslatef(0,0.0001f,0);

	glActiveTextureARB(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();



	glClientActiveTextureARB(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0,texcoords);
	
	glClientActiveTextureARB(GL_TEXTURE1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0,texcoords2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-10);
	
	
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT,indices);

	glClientActiveTextureARB(GL_TEXTURE0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	/*glBegin(GL_TRIANGLE_STRIP);
	//glTexCoord2f(0.0f,0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0,0,0);
	glMultiTexCoord2fARB(GL_TEXTURE1,0,0);
//#define ONLY_ALPHA
#ifdef ONLY_ALPHA
	glColor4f(1,1,1,alpha);
#elif ALL_ALPHA
	glColor4f(alpha,alpha,alpha,alpha);
#else
#endif
	glVertex3f(-2,-2,0);
	//glTexCoord2f(1.0f,0.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0,1,0);
	glMultiTexCoord2fARB(GL_TEXTURE1,1,0);
#ifdef ONLY_ALPHA
	glColor4f(1,1,1,alpha);
#elif ALL_ALPHA
	glColor4f(alpha,alpha,alpha,alpha);
#else
#endif
	glVertex3f(2,-2,0);
	//glTexCoord2f(0.0f,1.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0,0,1);
	glMultiTexCoord2fARB(GL_TEXTURE1,0,1);
#ifdef ONLY_ALPHA
	glColor4f(1,1,1,alpha);
#elif ALL_ALPHA
	glColor4f(alpha,alpha,alpha,alpha);
#else
#endif
	glVertex3f(-2,2,0);
	//glTexCoord2f(1.0f,1.0f);
	glMultiTexCoord2fARB(GL_TEXTURE0,1,1);
	glMultiTexCoord2fARB(GL_TEXTURE1,1,1);
#ifdef ONLY_ALPHA
	glColor4f(1,1,1,alpha);
#elif ALL_ALPHA
	glColor4f(alpha,alpha,alpha,alpha);
#else
#endif
	glVertex3f(2,2,0);
	glEnd();

	if(alpha>1.0f||alpha<0.0f)
		inc=-inc;
	alpha+=inc;*/


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

	
	pTexture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\27DoubleTexture\\waterfall.png");
	pTexture2=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\27DoubleTexture\\mask.png");

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	//glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_DST_COLOR,GL_ONE);
	//glEnable(GL_ALPHA_TEST);
	

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTexture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	
	glActiveTextureARB(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTexture2->bind();
#if 1
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_COLOR, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
#elif 0
	GLfloat constColor[4]={0.0f,0.0f,0.0f,0.8f};
	glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,constColor);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_ALPHA, GL_CONSTANT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_ALPHA, GL_PRIMARY_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#elif 0
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_PRIMARY_COLOR);
#else
#endif
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
	if(pTexture2)
		delete pTexture2;
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

#include <stdio.h>
/** 检查是否支持扩展 */
bool isExtensionSupported(const char* string)
{
	char *extension;			/**< 指向扩展字符串的指针 */
	char *end;				    /**< 最后一个字符指针 */
	int idx;

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;	

	/** 得到最后一个字符 */
	end = extension + strlen(extension);	

	/** 循环查找字符串string */
	while (extension < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extension, " ");

		/** 检查是否找到字符串string */
		if ( (strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}

		/** 当前串不是string,继续下一个字符串 */
		extension += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;   
}
int main(int argc, char **argv)
{
	EnableMemLeakCheck();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(640,480); 
	(void) glutCreateWindow("GL DoubleTexture");

	/** 检查是否支持扩展 */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{

		/** 获得函数指针地址 */
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
		printf("GL_ARB_multitexture success\n");
	}


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