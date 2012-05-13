#include "../yon_AndroidGLView.h"
#include "framework.h"
#include <GLES/gl.h>
#include "../log.h"

const char* LOG_TAG = "yon_AndroidGLView";

const static s32 ACTION_MASK = 255;
const static s32 ACTION_DOWN = 0;
const static s32 ACTION_UP = 1;
const static s32 ACTION_MOVE = 2;
const static s32 ACTION_POINTER_DOWN = 5;
const static s32 ACTION_POINTER_UP = 6;

void Java_yon_AndroidGLView_nativeOnSurfaceCreated(JNIEnv *pEnv, jobject obj, jint width, jint height, jstring apkFilePath, jstring sdcardPath){
	LOGD(LOG_TAG,"screen:{%d,%d},pEnv:%08x,nativeOnSurfaceCreated",width,height,pEnv);
	init(pEnv,width,height);
}
void Java_yon_AndroidGLView_nativeOnSurfaceChanged(JNIEnv *pEnv, jobject obj, jint w, jint h){
	Logger->debug("nativeOnSurfaceChanged->w:%d,h:%d\n",w,h);
	getEngine()->onResize(w,h);
}
void Java_yon_AndroidGLView_nativeOnDrawFrame(JNIEnv *pEnv, jobject obj){
	getEngine()->run();
	drawFrame();
}
void Java_yon_AndroidGLView_nativeOnPause(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnPause\n");
}
void Java_yon_AndroidGLView_nativeOnResume(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnResume\n");
}
jboolean Java_yon_AndroidGLView_nativeOnBack(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnBack\n");
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
		Logger->warn("can not find %s\n",className);
		return true;
	}
	//再找类中的方法
	jmethodID callbackDestroy = pEnv->GetMethodID(cls, "callbackDestroy", "()V");
	if (destroy == NULL) 
	{
		Logger->warn("no callbackDestroy function\n");
		return true;  
	}
	//回调java中的方法
	Logger->info("callbackDestroy function\n");
	destroy();
	pEnv->CallVoidMethod(obj, callbackDestroy);
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnTouch(JNIEnv *pEnv, jobject obj, jint iAction, jfloat fX, jfloat fY){
	//Logger->debug("nativeOnTouch:action:%d,%.2f,%.2f\n",iAction,fX,fY);
	SEvent evt;
	evt.type=ENUM_EVENT_TYPE_TOUCH;
	evt.touchInput.x=fX;
	evt.touchInput.y=fY;
	switch (iAction&ACTION_MASK){
	case ACTION_DOWN:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_DOWN;
		break;  
	case ACTION_UP:
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_UP;
		break;
	case ACTION_POINTER_UP:
		//evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_UP;
	case ACTION_POINTER_DOWN:
		break;
	case ACTION_MOVE: 
		evt.touchInput.type=ENUM_TOUCH_INPUT_TYPE_MOVE;
		break;
	}
	getEngine()->postEventFromUser(evt);
	return false;
}
void Java_yon_AndroidGLView_nativeOnSurfaceDestroy(JNIEnv *pEnv, jobject obj){
	Logger->debug("nativeOnSurfaceDestroy\n");
}

//很遗憾，测试发现此方法没有调用到
void JNI_OnUnload(JavaVM *jvm, void *reserved){
	//JNIEnv *env;
	//if(jvm->GetEnv(jvm,(void **)&env, JNI_VERSION_1_4)){
	//	return;
	//}
	//(*env)->DeleteWeakGlobalRef(env, Class_C);
	LOGD(LOG_TAG,"JNI_OnUnload");
}
