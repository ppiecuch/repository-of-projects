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


struct VertexData
{
	float u,v;
	float r,g,b,a;
	float nx,ny,nz;
	float x,y,z;
};

VertexData cube[] = 
{
	{1.0,0.0,	1.0,1.0,1.0,1.0,	0.0,0.0,1.0,	-0.5,-0.5,0.5},
	{0.0,0.0,	1.0,1.0,1.0,1.0,	0.0,0.0,1.0,	0.5,-0.5,0.5},
	{0.0,1.0,	1.0,1.0,1.0,1.0,	0.0,0.0,1.0,	0.5,0.5,0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	0.0,0.0,1.0,	-0.5,0.5,0.5},

	{1.0,0.0,	1.0,1.0,1.0,1.0,	1.0,0.0,0.0,	0.5,-0.5,0.5},
	{0.0,0.0,	1.0,1.0,1.0,1.0,	1.0,0.0,0.0,	0.5,-0.5,-0.5},
	{0.0,1.0,	1.0,1.0,1.0,1.0,	1.0,0.0,0.0,	0.5,0.5,-0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	1.0,0.0,0.0,	0.5,0.5,0.5},

	{0.0,0.0,	1.0,1.0,1.0,1.0,	0.0,0.0,-1.0,	-0.5,-0.5,-0.5},
	{0.0,1.0,	1.0,1.0,1.0,1.0,	0.0,0.0,-1.0,	-0.5,0.5,-0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	0.0,0.0,-1.0,	0.5,0.5,-0.5},
	{1.0,0.0,	1.0,1.0,1.0,1.0,	0.0,0.0,-1.0,	0.5,-0.5,-0.5},

	{1.0,0.0,	1.0,1.0,1.0,1.0,	-1.0,0.0,0.0,	-0.5,-0.5,-0.5},
	{0.0,0.0,	1.0,1.0,1.0,1.0,	-1.0,0.0,0.0,	-0.5,-0.5,0.5},
	{0.0,1.0,	1.0,1.0,1.0,1.0,	-1.0,0.0,0.0,	-0.5,0.5,0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	-1.0,0.0,0.0,	-0.5,0.5,-0.5},

	{0.0,1.0,	1.0,1.0,1.0,1.0,	0.0,-1.0,0.0,	-0.5,-0.5,-0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	0.0,-1.0,0.0,	0.5,-0.5,-0.5},
	{1.0,0.0,	1.0,1.0,1.0,1.0,	0.0,-1.0,0.0,	0.5,-0.5,0.5},
	{0.0,0.0,	1.0,1.0,1.0,1.0,	0.0,-1.0,0.0,	-0.5,-0.5,0.5},

	{0.0,0.0,	1.0,1.0,1.0,1.0,	0.0,1.0,0.0,	-0.5,0.5,-0.5},
	{0.0,1.0,	1.0,1.0,1.0,1.0,	0.0,1.0,0.0,	-0.5,0.5,0.5},
	{1.0,1.0,	1.0,1.0,1.0,1.0,	0.0,1.0,0.0,	0.5,0.5,0.5},
	{1.0,0.0,	1.0,1.0,1.0,1.0,	0.0,1.0,0.0,	0.5,0.5,-0.5},
};

PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB = NULL;
CTexture* texture=NULL;
CProjectiveTexture* pTexture=NULL;

#if 0
void BeginRender()
{
	static float planeS[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	static float planeT[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	static float planeR[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	static float planeQ[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
	glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

	glTexGenfv(GL_S,GL_EYE_PLANE,planeS);
	glTexGenfv(GL_T,GL_EYE_PLANE,planeT);
	glTexGenfv(GL_R,GL_EYE_PLANE,planeR);
	glTexGenfv(GL_Q,GL_EYE_PLANE,planeQ);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(matrix);                               // load our texture matrix
	//��Ⱦ���߾�����ˮ�ߣ����������ǵĲ������󣬺�ʱ����صĲ���������Ⱦ���ߣ�
	//��ʱ�Ѳ���Ҫ�Ĳ���ж�������Ǹÿ��ǵġ����嶥������Ӧ������ģ����ͼ����
	//��GL_MODELVIEW��ת�����������꣬Ȼ������������ģʽ�������������
	glMatrixMode(GL_MODELVIEW);             
}
#endif


GLdouble theta = 0;//��ת�Ƕ�
Matrix4x4f ObjectTransform, TextureTransform;
float MVmatrix[16];


void display(void) { 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLoadIdentity();
	//glRotatef(0,1.0f,0.0f,0.0f);
	//glRotatef(0,0.0f,1.0f,0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);

#if 1
	static bool p=true;
	if(p)
	{
		printf("--------------------Test---------------\n");
		glMatrixMode(GL_MODELVIEW);         
		printf("test\n");
		glLoadIdentity();
		//glTranslatef(3,2,3);
		//glOrtho(-200,200,-200,200,1,3000);
		glRotatef(90,0,0,1);
		glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
		GLfloat temp[]={2,0,0,1,0,1,0,2,0,0,1,3,0,0,0,1};
		glMultMatrixf(temp);
		glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);
		printf("==>%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
		//gluPerspective(60,2,1,100);
		//glOrtho(-5,5,-3,3,1,100);
		gluLookAt(1,2,3,6,5,4,0,1,0);
		//glLoadIdentity();
		//glScalef(1,2,3);
		glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
		glRotatef(60,1,1,1);
		glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
		GLfloat temp2[]={2,0,0,1,0,3,0,2,0,0,4,3,0,0,0,5};
		glMultMatrixf(temp2);
		glGetFloatv(GL_MODELVIEW_MATRIX,MVmatrix);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n\n",MVmatrix[0],MVmatrix[4],MVmatrix[8],MVmatrix[12],MVmatrix[1],MVmatrix[5],MVmatrix[9],MVmatrix[13],MVmatrix[2],MVmatrix[6],MVmatrix[10],MVmatrix[14],MVmatrix[3],MVmatrix[7],MVmatrix[11],MVmatrix[15]);
		p=false;
	}
#endif

	TextureTransform.setMatrix(MVmatrix);
	glPopMatrix();
	pTexture->textureProjection(TextureTransform);

	//glLoadIdentity();
	//gluLookAt(0.0, 1.0, 5.5, 0.0, 1.0, -3.0, 0.0, 1.0, 0.0);
	//glRotatef(theta, 0, 1, 0);
	//glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,2.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glRotatef(rotx, 0.0, 1.0, 0.0);
	glInterleavedArrays(GL_T2F_C4F_N3F_V3F,0,cube);
	glDrawArrays(GL_QUADS,0,24);

	glDisable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	glutSolidSphere(0.1,16,16);
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_FRONT);
	//theta +=0.02;
	//if(theta >= 360)
	//	theta = 0;
	//glPopMatrix();
	glutSwapBuffers();
}
#include <stdio.h>
/** ����Ƿ�֧����չ */
bool isExtensionSupported(const char* string)
{
	char *extension;			/**< ָ����չ�ַ�����ָ�� */
	char *end;				    /**< ���һ���ַ�ָ�� */
	int idx;

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;	

	/** �õ����һ���ַ� */
	end = extension + strlen(extension);	

	/** ѭ�������ַ���string */
	while (extension < end)
	{
		/** �ҵ��ո�ǰ��һ���ַ��� */
		idx = strcspn(extension, " ");

		/** ����Ƿ��ҵ��ַ���string */
		if ( (strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}

		/** ��ǰ������string,������һ���ַ��� */
		extension += (idx + 1);
	}
	/** û���ҵ�,�򷵻�false */
	return false;   
}
void init()
{
	/** ����Ƿ�֧����չ */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{

		/** ��ú���ָ���ַ */
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		printf("GL_ARB_multitexture success\n");
	}

	glShadeModel(GL_SMOOTH);
	glClearColor(0.1, 0.2, 0.3, 1); 

	//Enable front face culling so we can see into the box
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0,0,640,480);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f,(double)640/(double)480,0.1f,100.0f);
	//glMatrixMode(GL_MODELVIEW);
	//gluLookAt(0.0,0.0,2.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5f,0.5,-0.5f,0.5f,1.0f,15.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,2.0,0.0,0.0,0.0,0.0,1.0,0.0);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	texture=new CTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\21GLProjectiveMultiTexture\\wood.png");
	texture->bind();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	//�뵱ǰģ����ͼ����������ĳ˷������Ѿ����������Ӿ�ƽ�淽��ʽ�С�����Ҫ���ľ���ȷ���������ģ����ͼ�����Ѿ��������glTexGenfvʱ�������á�
	pTexture=new CProjectiveTexture("D:\\Development\\Workspace\\VS2008\\OpenGLES\\21GLProjectiveMultiTexture\\test.png");
	pTexture->bind();
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
	//glutPostRedisplay ��ǵ�ǰ������Ҫ���»��ơ�ͨ��glutMainLoop��һ��ѭ��ʱ��������ʾ�����ص���������ʾ���ڵ�������塣
	//��ε���glutPostRedisplay������һ����ʾ�ص�ֻ������һ��������ʾ�ص��� 
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
	if(texture)
		delete texture;
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(640,480); 
	(void) glutCreateWindow("GL ProjectiveMultiTexture");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	Singleton::GetInstance();//������Դ�ͷ�

	glutMainLoop();

	//destroy();

	return 0;             /* ANSI C requires main to return int. */
}