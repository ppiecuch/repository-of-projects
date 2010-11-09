#include <jni.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <zip.h>
#include "log.h"
#include "androidgl.h"
#include "math/point3f.h"
#include "math/sphere.h"
#include "math/cuboid.h"
#include "util.h"

/***************************************/
/*        长方体与球体(光照教程)        */
/***************************************/

float cameraPosition[]={0,5,40};
static float cameraAngle=0;
static float cameraSin;
static float cameraCos;
static void rotateCamera()
{
	cameraAngle-=1;
	if(cameraAngle<=0)
		cameraAngle+=360;
	cameraSin=sinf(cameraAngle*M_PI/180.0);
	cameraCos=cosf(cameraAngle*M_PI/180.0);
	float _x= cameraCos - 40*cameraSin;
	float _z= cameraSin + 40*cameraCos;
	cameraPosition[0]=_x;
	cameraPosition[2]=_z;
}

static GLfloat axeVertexArray[][3] = {
	{0,0,0},
	{10,0,0},
	{0,0,0},
	{0,10,0},
	{0,0,0},
	{0,0,10}
};
static GLubyte axeColorArray[][3] = {
  255, 0, 0, 255,
  255, 0, 0, 255,
  0, 255, 0, 255,
  0, 255, 0, 255,
  0, 0, 255, 255,
  0, 0, 255, 255
};

#define RADIUS 8.0f
#define HORIZONAL_STEPS 24
#define PORTRAIT_STEPS 12
#define CUBOID_LENGTH 8
#define CUBOID_WIDTH 8
#define CUBOID_HEIGHT 8
Sphere* sphere;
Cuboid* cuboid;
zip* APKArchive;
GLuint textures[2];
float spherePos[]={0,10,0};
float cuboidPos[]={0,-8,0};

GLfloat materialSpecular[]={0.5,0.5,0.5,1.0};//镜面颜色
GLfloat materialShininess[]={1.0};//镜面指数
GLfloat materialDiffuse[]={0.1,0.1,0.1,1.0};//散射颜色
GLfloat materailEmission[]={1.0,0,0,1.0};//放射颜色
GLfloat lightAmbient[]={1.0,1.0,1.0,1.0};
GLfloat lightDiffuse[]={0.8,0.8,0.8,1.0};
GLfloat lightSpecular[]={0.9,0.9,0.9,1.0};
GLfloat lightPosition[]={20,0,0,0.0};//将w设置为0来定义平行光
GLfloat lightSpotDirection[]={-20,0,0};//聚光灯方向,指向原点
GLfloat lightSpotExponent[]={0.5};//聚光指数
GLfloat lightSpotCutoff[]={60};//聚光灯的切角
GLfloat lightModelAmbient[]={0.01,0.01,0.01,1.0};//光照模型,整个场景的环境光的RGBA强度
GLfloat lightModelTwoSize[]={GL_FALSE};//指定单面或双面光照



static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;


void appInit(JNIEnv*  env, jobject thiz, jstring apkPath)
{
	LOGI("AndroidGL","apkPath:s%",apkPath);
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);
	/*创建球体*/
	sphere=new Sphere(RADIUS,HORIZONAL_STEPS,PORTRAIT_STEPS);
	sphere->toString();
	/*创建长方体*/
	cuboid=new Cuboid(CUBOID_LENGTH,CUBOID_WIDTH,CUBOID_HEIGHT);
	cuboid->toString();
	/*创建纹理*/
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",*str);
	APKArchive=loadAPK(str);

	int width,height,colorType;
	png_byte* data;
	LOGI("AndroidGL","apply %d texture name",sizeof(textures)/sizeof(textures[0]));
	glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
	/*创建纹理1*/
	data=readPngData("assets/androidgl/earth.png",width,height,colorType,APKArchive);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, (GLvoid*) data);
	/*创建纹理2*/
	data=readPngData("assets/androidgl/wood.png",width,height,colorType,APKArchive);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, (GLvoid*) data);
	delete[] data;

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	printGLString("ShadeModel",GL_SHADE_MODEL);//着色模型

	/*纹理图像的最大尺寸(没有考虑纹理的内部存储格式)。*/
	printGLInteger("GL_MAX_TEXTURE_SIZE",GL_MAX_TEXTURE_SIZE);
	printGLInteger("GL_MAX_TEXTURE_STACK_DEPTH",GL_MAX_TEXTURE_STACK_DEPTH);
	/*查询当前OpenGL实现所支持的纹理单位的数量。任何OpenGL实现至少必须支持2个纹理单位。*/
	printGLInteger("GL_MAX_TEXTURE_UNITS",GL_MAX_TEXTURE_UNITS);

	LOGI("AndroidGL","Init",NULL);
}

void appResize(int width,int height)
{
	/*视口变换*/
	glViewport(0, 0, width, height);
	/*投影变换*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/(float)height,0.1f,200);
	/*模型矩阵*/
	glMatrixMode(GL_MODELVIEW);
	/*启用剔除功能*/
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//创建光源
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
//	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lightSpotDirection);
//	glLightfv(GL_LIGHT0,GL_SPOT_EXPONENT,lightSpotExponent);
//	glLightfv(GL_LIGHT0,GL_SPOT_CUTOFF,lightSpotCutoff);
	//选择光照模型
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lightModelAmbient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,lightModelTwoSize);
	//启用光照
	glEnable(GL_LIGHTING);
	//启用光源
	glEnable(GL_LIGHT0);

	/*启用平滑模式,默认启用*/
	glShadeModel(GL_SMOOTH);
	/*保持法线向量规范化,必要时使用*/
	glEnable(GL_NORMALIZE);

	//启用深度测试
	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);

	/*设置线条大小*/
	glLineWidth(4.0);

	LOGI("AndroidGL","Resize",NULL);
}

void appRender(long tick, int width, int height)
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraPosition[0],cameraPosition[1],cameraPosition[2],0,0,0,0,1,0);
	//LOGI("Camera","{%.2f,%.2f,%.2f,%.2f,%.2f}",cameraSin,cameraCos,cameraPosition[0],cameraPosition[1],cameraPosition[2]);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	/*下面几个函数不可放于此处,不然灯光失效*/
//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,axeVertexArray);
		glColorPointer(3,GL_UNSIGNED_BYTE,0,axeColorArray);
		glMaterialfv(GL_FRONT,GL_EMISSION,materailEmission);
		glDrawArrays(GL_LINES,0,2);
		glDrawArrays(GL_LINES,2,2);
		glDrawArrays(GL_LINES,4,2);
		glDisableClientState(GL_COLOR_ARRAY);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	glTranslatef(spherePos[0],spherePos[1],spherePos[2]);
	/**
	 * 不明事故:上面一行如果将spherePos改为Point3f*类型,然后改为如下调用:
	 * glTranslatef(spherePos->x,sphererPos->y,spherePos->z);
	 * 会报fault addr错误
	 */
	glRotatef(angle,0,1,0);
	glVertexPointer(3,GL_FLOAT,0,sphere->getVertexs());
	glTexCoordPointer(2, GL_FLOAT, 0, sphere->getTexCoords() );
	glNormalPointer(GL_FLOAT,0,sphere->getNormals());
//	glEnable(GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D,textures[0]);
//	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
	glMaterialfv(GL_FRONT,GL_SHININESS,materialShininess);
//	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffuse);
	for(int i=0;i<PORTRAIT_STEPS;i++)
		for(int j=0;j<HORIZONAL_STEPS;j++)
			glDrawArrays(GL_TRIANGLE_FAN,(i*HORIZONAL_STEPS+j)<<2,4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(cuboidPos[0],cuboidPos[1],cuboidPos[2]);
	glRotatef(angle,1,1,0);
	glVertexPointer(3,GL_FLOAT,0,cuboid->getVertexs());
	glTexCoordPointer(2, GL_SHORT, 0, cuboid->texCoords);
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	glNormalPointer(GL_SHORT,0,cuboid->normals);
//	glMaterialfv(GL_FRONT,GL_SPECULAR,materialSpecular);
//	glMaterialfv(GL_FRONT,GL_SHININESS,materialShininess);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialDiffuse);
	for(int i=0;i<6;i++)
			glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,cuboid->vertexIndices[i]);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
	angle+=1.5f;
	rotateCamera();
}


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
    appInit(env,thiz,apkPath);
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_gl_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    appResize(w,h);
    LOGI("AndroidGL","resize w=%d h=%d", w, h);
}
void
Java_org_tinder_studio_gl_AndroidGLView_nativePause(JNIEnv *env, jobject thiz)
{
	LOGI("AndroidGL","Pause",0);
}

static JNINativeMethod methods[] = {

  {"nativeInit", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeInit },
  {"nativeDone", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeDone },
  {"nativeRender", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeRender },
  {"nativeResize", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeResize }

};

static const char *classPathName = "org/tinder/studio/gl/AndroidGLView";

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }
  return JNI_TRUE;
}

