/**
*
* @author Hong Zhibiao
* @email  babala_234@163.com
* @brief Shadow Mapping
*
* @operation:
* p:alternately use GL_NEAREST or GL_LINEAR with generating shadow map
*
* @perplexity:
* why shadow texture's size can't be larger than camera view port's size
*/

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifdef GL_ARB_shadow    
#define GL_TEXTURE_COMPARE_MODE      GL_TEXTURE_COMPARE_MODE_ARB    
#define GL_TEXTURE_COMPARE_FUNC      GL_TEXTURE_COMPARE_FUNC_ARB    
#define GL_DEPTH_TEXTURE_MODE        GL_DEPTH_TEXTURE_MODE_ARB    
#define GL_COMPARE_R_TO_TEXTURE      GL_COMPARE_R_TO_TEXTURE_ARB    
#endif   


#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "esMatrix.h"
#pragma comment(lib, "Math.lib")

#include "FPSCounter.h"
#include "global.h"
#include <string.h>
#include <stdio.h>
#pragma comment(lib,"Utility.lib")
FPSCounter fpsCounter;

using namespace std;


// window size
int windowWidth  = 512;
int windowHeight = 512;

bool bAmbientShadowAvailable=false;
bool first=true;

// global variable
int    shadowMapSize = 512;//64,128,256,512都支持，但1024不支持为什么？
GLuint shadowMapTexture=-1;

float lightPosition[4] ={2.0f, 2.f, 0.f, 1.f};
float lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
float lightDiffuse[] = { 0.35f, 0.35f, 0.35f, 1.0f };
float lightSpecular[] = { 0.f, 0.f, 0.f, 0.f };

ESMatrix lightProjectionMatrix;
ESMatrix lightViewMatrix;
ESMatrix textureMatrix;

float cameraPosition[4]={-1.5f, 2.5f, 5.f, 1.f};

float white[4]={1.f,1.f,1.f,1.f};
float gray[4]={0.3f,0.3f,0.3f,1.f};
float black[4]={0.f,0.f,0.f,1.f};



enum ENUM_MODE{
	ENUM_MODE_NONE=0,
	ENUM_MODE_ROTATE
};
GLuint parameter=GL_NEAREST;


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



void drawScene ()
{
	glFrontFace(GL_CW);
	glColor3f(0.5f,0,0);
	glutSolidTeapot(0.5);

	glFrontFace(GL_CCW);
	glColor3f(0,0.5f,0);
	glPushMatrix();
	glTranslatef(0,0,-1);
	glutSolidCube(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,0,0.5f);
	glTranslatef(1.5f,0.5f,0);
	glutSolidSphere(0.5f,16,16);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f,0.5f,0);
	glTranslatef(0,0,1);
	glRotatef(-90,0,0,1);
	glRotatef(-90,1,0,0);
	glutSolidCone(0.5f,0.5f,16,16);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f,0,0.5f);
	glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);
	glutSolidSphere(0.1f,4,4);
	glPopMatrix();

	
	
	glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_QUADS);
	glNormal3f(0.f,1.f,0.f);
	glVertex3f(-4.f, -1.5f, 4.f);
	glVertex3f( 4.f, -1.5f, 4.f);
	glVertex3f( 4.f, -1.5f,-4.f);
	glVertex3f(-4.f, -1.5f,-4.f);
	glEnd();

}

void initShadowMap();

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

void drawShadowTexture(){

	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f, 0.f);
	glTexCoord2f(0,0);
	glVertex3f(-0.7f, -1.0f, 0.f);
	glTexCoord2f(1,0);
	glVertex3f(-0.7f, -0.7f, 0.f);
	glTexCoord2f(1,1);
	glVertex3f(-1.0f, -0.7f, 0.f);
	glTexCoord2f(0,1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// callback to draw graphic primitives
void display ()
{
	if(first)
	{
		//阴影贴图的生成必须在render中进行，不能在init中进行，否则有时会出现阴影错乱的情况
		initShadowMap();

		glViewport(0,0,windowWidth,windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (float)windowWidth/windowHeight, 0.01f, 100.0f);
		first=false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glRotatef(rotx, 0.0, 1.0, 0.0);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);

	//如果ARB_shadow_ambient受到硬件支持，那么我们直接指定GL_TEXTURE_COMPARE_FAIL_VALUE_ARB为我们的阴影颜色，
	//这样就不用先渲染阴影部分了，然而大部分硬件并不支持此功能
	if(bAmbientShadowAvailable==false)
	{
		//使用灰色渲染阴影
		glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);
		glLightfv(GL_LIGHT0, GL_SPECULAR, black);

		drawScene();

		//开启Alpha测试，因为阴影部分的alpha为0
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.9f);
	}

	//使用白色渲染明亮部分
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	
	glEnable(GL_TEXTURE_2D);

	//纹理自动生成由投影纹理矩阵顶点的世界坐标（即经过Model Transform的顶点）相乘得到
	//当我们对相机进行方位变换时，Model Transform受到影响，
	//因此我们将阴影纹理生成放在render中进行
	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, &textureMatrix.m[0][0]);
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, &textureMatrix.m[1][0]);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, &textureMatrix.m[2][0]);
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, &textureMatrix.m[3][0]);
	glEnable(GL_TEXTURE_GEN_Q);


	drawScene();


	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_COLOR_MATERIAL);
	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);

	printFPS();
	drawShadowTexture();

	// swap buffer
	glutSwapBuffers();
}


// callback to resize the size of the viewport
void resize(int w, int h)
{
	glViewport (0, 0, w, h);
	windowWidth = w; windowHeight = h;

	//Calculate & save matrices
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 0.01f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glPopMatrix();
}


// mysterious callback function ...
void idle()
{
	glutPostRedisplay();
}


// callback to manage keyboard interactions
void key(unsigned char key, int x, int y)
{
	switch (key) {
	case 'p':
		if(parameter==GL_NEAREST)
			parameter=GL_LINEAR;
		else
			parameter=GL_NEAREST;
		first=true;
		break;
	case 'q':
	case 'Q':
	case 27: // Esc
		exit (EXIT_SUCCESS);
		break;
	}
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

void initShadowMap ()
{
	if (IsExtensionSupported("GL_ARB_shadow_ambient",(char*)glGetString(GL_EXTENSIONS)))
		bAmbientShadowAvailable = true;
	printf("GL_ARB_shadow_ambient supported:%d\n",bAmbientShadowAvailable);


	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1.0f, 1.f, 10.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, &lightProjectionMatrix.m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		lightPosition[0], lightPosition[1], lightPosition[2],
		1,0,0,
		-1,1,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, &lightViewMatrix.m[0][0]);

	//set states
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);//默认为GL_LESS，改为LEQUAL是为了渲染明亮部分时通过深度测试
	//Draw back faces into the shadow map
	//即只有背光有面片才绘制
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	
	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// avoid Z-fighting
	glPolygonOffset(2, 4);
	glEnable( GL_POLYGON_OFFSET_FILL );


	//开始创建阴影贴图，先清空深度缓冲
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&lightProjectionMatrix.m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&lightViewMatrix.m[0][0]);


	

	//渲染场景
	drawScene();

	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	//指定纹理边缘颜色，以便混合
	GLfloat borderColor[4] = {1,1, 1, 1};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
	//使用GL_LINEAR效果会好一点点
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//纹理坐标的r分量表示光源到片元的距离，阴影贴图的深度值d表示从光源到最近的背光片元的距离
	//对两者进行比较，如果r<=d，则片元被光照射到，所以对应的颜色值为(1.0,1.0,1.0,1.0)
	//否则为(0,0,0,0)
	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	if(bAmbientShadowAvailable)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.5f);
	//Read the depth buffer into the shadow map texture
	//GL_DEPTH_COMPONENT
	//Each element is a single depth value. The GL converts it to floating point, multiplies by the signed scale factor GL_DEPTH_SCALE, adds the signed bias GL_DEPTH_BIAS, and clamps to the range [0,1] (see glPixelTransfer). 
	//从深度缓冲区中复制深度值到一个纹理中，作为阴影贴图使用（OpenGL1.4才支持），两种方法：
	//第一种：(诡异：调整阴影图大小后，下面glCopyTexSubImage2D必须有，否则会导致阴影错乱)
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	//glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,shadowMapSize,shadowMapSize);
	//第二种：
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,shadowMapSize,shadowMapSize,0);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	//计算投影纹理矩阵
	esMatrixLoadIdentity(&textureMatrix);
	esTranslate(&textureMatrix,0.5f,0.5f,0.5f);
	esScale(&textureMatrix,0.5f,0.5f,0.5f);
	esMatrixMultiply(&textureMatrix,&lightProjectionMatrix,&textureMatrix);
	esMatrixMultiply(&textureMatrix,&lightViewMatrix,&textureMatrix);

	//因为我们将投影纹理矩阵用于纹理自动生成，而OpenGL的矩阵是以列优先排列，
	//为了将4个列分别用于{s,t,r,q}纹理坐标生成，这里对矩阵进行转置
	esMatrixTranspose(&textureMatrix,&textureMatrix);

	//restore states
	glViewport(0,0,windowWidth,windowHeight);
	glDisable( GL_POLYGON_OFFSET_FILL );
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);
	glEnable(GL_NORMALIZE);
	//启用颜色材质，开启后让你在激活光照的情况下用glColor函数给物体上色
	glEnable(GL_COLOR_MATERIAL);
}

void init()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.1, 0.2, 0.3, 1); 
}

int main (int argc, char **argv)
{
	//initGlut (argc, argv);
	//initGl ();
	EnableMemLeakCheck();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100,100); 
	glutInitWindowSize(windowWidth,windowHeight); 
	(void) glutCreateWindow("GL ShadowMapping");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop ();
	return (EXIT_SUCCESS);
}


