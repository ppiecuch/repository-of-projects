#include <jni.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <zip.h>
#include "log.h"
#include "androidgl.h"
#include "math/point3f.h"
#include "math/sphere.h"
#include "util.h"

/***************************************/
/*             球体(纹理教程)           */
/***************************************/

#define ONE 1
#define RADIUS 8.0f
#define HORIZONAL_STEPS 24
#define PORTRAIT_STEPS 12
Sphere* sphere;
zip* APKArchive;
GLuint texture;

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
	/*创建纹理*/
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",str);
	APKArchive=loadAPK(str);

	int width,height,colorType;
	png_byte* data=readPngData("assets/androidgl/earth.png",width,height,colorType,APKArchive);

	/*创建纹理*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, (GLvoid*) data);
	delete[] data;

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	/*纹理图像的最大尺寸(没有考虑纹理的内部存储格式)。*/
	printGLInteger("GL_MAX_TEXTURE_SIZE",GL_MAX_TEXTURE_SIZE);
	printGLInteger("GL_MAX_TEXTURE_STACK_DEPTH",GL_MAX_TEXTURE_STACK_DEPTH);
	/*查询当前OpenGL实现所支持的纹理单位的数量。任何OpenGL实现至少必须支持2个纹理单位。*/
	printGLInteger("GL_MAX_TEXTURE_UNITS",GL_MAX_TEXTURE_UNITS);

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
}

void appRender(long tick, int width, int height)
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,40,0,0,-50,0,1,0);
	glRotatef(angle,0,1,0);
	angle+=0.5f;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,sphere->getVertexs());
	//glEnableClientState(GL_COLOR_ARRAY);
	//glColorPointer(4,GL_UNSIGNED_BYTE,0,colorArray);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, sphere->getTexCoords() );
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	for(int i=0;i<PORTRAIT_STEPS;i++)
		for(int j=0;j<HORIZONAL_STEPS;j++)
			glDrawArrays(GL_TRIANGLE_FAN,(i*HORIZONAL_STEPS+j)<<2,4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_COLOR_ARRAY);
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

static const char *classPathName = "org/tinder/studio/gl/AndroidGLView";
static JNINativeMethod methods[] = {

  {"nativeInit", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeInit },
  {"nativeDone", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeDone },
  {"nativeRender", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeRender },
  {"nativeResize", "()", (void*)Java_org_tinder_studio_gl_AndroidGLView_nativeResize }

};
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


