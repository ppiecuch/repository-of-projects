#include "yon_AndroidGLView.h"
#include "include/yon.h"
#include "log.h"
#include <GLES/gl.h>
#include <locale.h>

using namespace yon;
using namespace yon::core;
using namespace yon::debug;
using namespace yon::video;
using namespace yon::scene;

IYonEngine* engine=NULL;
IVideoDriver* driver=NULL;
ISceneManager* sceneMgr=NULL;

#define JNI_VERSION_1_1 0x00010001
#define JNI_VERSION_1_2 0x00010002
#define JNI_VERSION_1_4 0x00010004
#define JNI_VERSION_1_5 0x00010005
#define JNI_VERSION_1_6 0x00010006

const char* LOG_TAG = "yon_AndroidGLView";

void checkJNIVersion(JNIEnv *pEnv){
	jint version= pEnv->GetVersion();
	switch(version){
	case JNI_VERSION_1_1:
		LOGI(LOG_TAG,"jni version:1.1");
		break;
	case JNI_VERSION_1_2:
		LOGI(LOG_TAG,"jni version:1.2");
		break;
	case JNI_VERSION_1_4:
		LOGI(LOG_TAG,"jni version:1.4");
		break;
	case JNI_VERSION_1_5:
		LOGI(LOG_TAG,"jni version:1.5");
		break;
	case JNI_VERSION_1_6:
		LOGI(LOG_TAG,"jni version:1.6");
		break;
	default:
		LOGI(LOG_TAG,"jni version:%d",version);
		break;
	}
}

void Java_yon_AndroidGLView_nativeOnSurfaceCreated(JNIEnv *pEnv, jobject obj, jstring apkFilePath, jstring sdcardPath){
	checkJNIVersion(pEnv);
	//setlocale(LC_CTYPE,"UTF-8");

	SYonEngineParameters params;
	engine=CreateEngine(params);

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();

	IEntity* cube=geometryFty->createCube();
	IModel* model=sceneMgr->addModel(cube);
	cube->drop();

	LOGD(LOG_TAG,"nativeOnSurfaceCreated");
}
void Java_yon_AndroidGLView_nativeOnSurfaceChanged(JNIEnv *pEnv, jobject obj, jint w, jint h){
	LOGD(LOG_TAG,"nativeOnSurfaceChanged->w:%d,h:%d",w,h);
	core::dimension2du size;
	size.w = w;
	size.h = h;
	driver->onResize(size);
}
void Java_yon_AndroidGLView_nativeOnDrawFrame(JNIEnv *pEnv, jobject obj){
	//LOGD(LOG_TAG,"nativeOnDrawFrame");
	//glClearColor(0.1f,0.2f,0.3f,1);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	driver->begin(true,COLOR_GRAY);

	sceneMgr->render(driver);

	//smgr->drawAll();
	//guienv->drawAll();

	//driver->endScene();
	//Sleep(20);
	//printf("run\n");
	//num++;
	//Logger->debug("%s\n","test你好");
	//logger->info("TEST\n");
	driver->end();
}
void Java_yon_AndroidGLView_nativeOnPause(JNIEnv *pEnv, jobject obj){
	LOGD(LOG_TAG,"nativeOnPause");
}
void Java_yon_AndroidGLView_nativeOnResume(JNIEnv *pEnv, jobject obj){
	LOGD(LOG_TAG,"nativeOnResume");
}
jboolean Java_yon_AndroidGLView_nativeOnBack(JNIEnv *pEnv, jobject obj){
	LOGD(LOG_TAG,"nativeOnBack");
	//SINCE JDK/JRE 1.2:
	//In JDK 1.1, FindClass searched only local classes in CLASSPATH. The resulting classes did not have a class loader.
	//The Java security model has been extended to allow non-system classes to load and call native methods. 
	//In the Java 2 Platform, FindClass locates the class loader associated with the current native method. 
	//If the native code belongs to a system class, no class loader will be involved. Otherwise, 
	//the proper class loader will be invoked to load and link the named class.
	//When FindClass is called through the Invocation Interface, there is no current native method or its associated class loader. 
	//In that case, the result of ClassLoader.getBaseClassLoader is used. 
	//This is the class loader the virtual machine creates for applications, 
	//and is able to locate classes listed in the java.class.path property.
	
	
	const char* className="yon/AndroidGLView";
	jclass cls = pEnv->FindClass(className);
	 if (cls == NULL) {
        LOGW(LOG_TAG, "can not find %s",className);
        return true;
    }
     //再找类中的方法
     jmethodID destroy = pEnv->GetMethodID(cls, "callbackDestroy", "()V");
     if (destroy == NULL) 
     {
         LOGW(LOG_TAG,"no callbackDestroy function");
         return true;  
     }
     //回调java中的方法
     LOGI(LOG_TAG,"callbackDestroy function");
	 engine->drop();
     pEnv->CallVoidMethod(obj, destroy);
	 return true;
}
jboolean Java_yon_AndroidGLView_nativeOnTouch(JNIEnv *pEnv, jobject obj, jint iAction, jfloat fX, jfloat fY){
	LOGD(LOG_TAG,"nativeOnTouch:action:%d,%.2f,%.2f",iAction,fX,fY);
	return false;
}
void Java_yon_AndroidGLView_nativeOnSurfaceDestroy(JNIEnv *pEnv, jobject obj){
	LOGD(LOG_TAG,"nativeOnSurfaceDestroy");
}