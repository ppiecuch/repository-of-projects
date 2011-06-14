#include "androidglview.h"
#include <GLES/gl.h>

#include <zipint.h>
#include <zip.h>
#include "common/log.h"
#include "common/ziputil.h"
#include "common/pngutil.h"
#include "common/glutil.h"
#include "common/gbnunicode.h"
#include "fontbrush.h"


static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;

zip* APKArchive;
FontBrush* brush;

//英文
//const static char* fontPath="assets/font/Test.ttf";
//const static char* text[]={"abcdefghijknlmnopq","rstuvwxyz","ABCDEFGHIJKLMNOPQ","RSTUVWXYZ"};

//中文
int len;
const static char* fontPath="assets/font/SIMKAI.TTF";
const static char* gb="abc中华人民共和国万岁!";
unsigned short* unicode;

void initFontBrush( JNIEnv*  env, jobject thiz, jstring apkPath)
{
	LOGI("AndroidGL","apkPath:s%",apkPath);
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	LOGI("AndroidGL","str:s%",*str);
	APKArchive=loadAPK(str);

	zip_file* file;
	file=zip_fopen(APKArchive, fontPath, 0);
	if(!file)
	{
		LOGE("AndroidGL","Error opening %s from APK",fontPath);
		return;
	}
	unsigned long fileSize=file->bytes_left;
	FT_Byte* fileBase=new FT_Byte[fileSize];
	zip_fread(file, fileBase, fileSize);
	LOGI("AndroidGLView","zip_fread:%ld",fileSize);

	brush=new FontBrush(fileBase,fileSize);

	delete[] str;
	delete[] fileBase;

	zip_fclose(file);

	LOGI("AndroidGLView","initFontBrush completed");
}

void initUnicode(){
	len=gb2Unicode(gb,unicode);
	LOGI("AndroidGLView","len:%d:unicode:%lx",len,unicode[0]);
}

void appInit( JNIEnv*  env, jobject thiz, jstring apkPath)
{
	initFontBrush(env,thiz,apkPath);
	initUnicode();
	glShadeModel(GL_SMOOTH|GL_FLAT);
	glClearColor(0.2f, 0.2f, 0.5f, 0.5f);
}

void appResize(int width,int height)
{
	/*视口变换*/
	glViewport(0, 0, width, height);
	/*投影变换*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0,(float)width/(float)height,0.1f,200);
	glOrthof(-sWindowWidth/2,sWindowWidth/2,-sWindowHeight/2,sWindowHeight/2,100,-100);
	/*模型矩阵*/
	glMatrixMode(GL_MODELVIEW);
	/*启用剔除功能*/
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	LOGI("AndroidGLView","Resize");
}

void appRender(long tick, int width, int height)
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,10,40,0,0,0,0,1,0);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//英文
	//for(int i=0;i<4;i++)
	//	brush->drawInLine(text[i],0xFFFFFFFF,16,10-sWindowWidth/2,sWindowHeight/2-40-i*16);
	//中文
	brush->drawInLine(unicode,len,0xFFFFFFFF,16,10-sWindowWidth/2,sWindowHeight/2-40);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
}


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
    appInit(env,thiz,apkPath);
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_AndroidGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_AndroidGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    appResize(w,h);
    LOGI("AndroidGLView", "resize w=%d h=%d", w, h);
}

void
Java_org_tinder_studio_AndroidGLView_nativePause(JNIEnv *env, jobject thiz)
{
}

jboolean
Java_org_tinder_studio_AndroidGLView_nativeOnBackDown
  (JNIEnv *env, jobject thiz)
{
	return false;
}

void
Java_org_tinder_studio_AndroidGLView_nativePress
  (JNIEnv *env, jobject thiz, jfloat x, jfloat y)
{
}

void 
Java_org_tinder_studio_AndroidGLView_nativeRelease
  (JNIEnv *env, jobject thiz, jfloat x, jfloat y)
{
}

/*
* Register several native methods for one class.
*/

int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    LOGI("AndroidGLView", "RegisterNatives start for '%s'", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGI("AndroidGLView", "Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGI("AndroidGLView", "RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static JNINativeMethod methods[] = {

  {"nativeInit", "()", (void*)Java_org_tinder_studio_AndroidGLView_nativeInit },
  {"nativeDone", "()", (void*)Java_org_tinder_studio_AndroidGLView_nativeDone },
  {"nativeRender", "()", (void*)Java_org_tinder_studio_AndroidGLView_nativeRender },
  {"nativeResize", "()", (void*)Java_org_tinder_studio_AndroidGLView_nativeResize }

};

static const char *classPathName = "org/tinder/studio/AndroidGLView";

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
