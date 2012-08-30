#include "../yon_AndroidGLView.h"
#include <GLES/gl.h>
#include <cstdio>
#include <exception>
using namespace std;
#include "../log.h"

const char* LOG_TAG = "yon_AndroidGLView";
//²âÊÔset_terminate
void unhandler()
{
	LOGD(LOG_TAG,"unexpected");
}
void handler()
{
	LOGD(LOG_TAG,"error");
}
void ThrowJNIException(const char* pzFile, int iLine,const char* pzMessage){
	LOGD(LOG_TAG,"%s,%d,%s",pzFile,iLine,pzMessage);
}

#define THROW_JAVA_EXCEPTION(_info) \
	ThrowJNIException(__FILE__,__LINE__,_info);



void Java_yon_AndroidGLView_nativeOnSurfaceCreated(JNIEnv *pEnv, jobject obj, jboolean first,jint width, jint height, jstring apkFilePath, jstring sdcardPath){
	LOGD(LOG_TAG,"first:%s,screen:{%d,%d},pEnv:%08x,nativeOnSurfaceCreated",first?"true":"false",width,height,pEnv);
	set_unexpected (unhandler); 
	std::set_terminate(handler);
	*(int *)0 = 0;
	//throw exception();
	/*try{
		throw exception();
	}catch(...){
		THROW_JAVA_EXCEPTION("error");
	}*/
}
void Java_yon_AndroidGLView_nativeOnSurfaceChanged(JNIEnv *pEnv, jobject obj, jint w, jint h){
}
void Java_yon_AndroidGLView_nativeOnDrawFrame(JNIEnv *pEnv, jobject obj){
}
void Java_yon_AndroidGLView_nativeOnPause(JNIEnv *pEnv, jobject obj){
}
void Java_yon_AndroidGLView_nativeOnResume(JNIEnv *pEnv, jobject obj){
}
void Java_yon_AndroidGLView_nativeOnDestroy(JNIEnv *pEnv, jobject obj){
}
jboolean Java_yon_AndroidGLView_nativeOnBack(JNIEnv *pEnv, jobject obj){
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnTouch(JNIEnv *pEnv, jobject obj, jint iAction, jint id, jfloat x, jfloat y, jint count){
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnMove(JNIEnv *pEnv, jobject obj, jint iAction, jintArray ids, jfloatArray xs, jfloatArray ys, jint count){
	return true;
}
jboolean Java_yon_AndroidGLView_nativeOnUI(JNIEnv *pEnv, jobject obj, jint msg, jobjectArray args){
	return true;
}
void Java_yon_AndroidGLView_nativeDebug(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	LOGD(LOG_TAG,"%s",text);
}
void Java_yon_AndroidGLView_nativeInfo(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	LOGI(LOG_TAG,"%s",text);
}
void Java_yon_AndroidGLView_nativeWarn(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	LOGW(LOG_TAG,"%s",text);
}
void Java_yon_AndroidGLView_nativeError(JNIEnv *pEnv, jobject obj, jstring str){
	const char* text= pEnv->GetStringUTFChars(str, 0);
	LOGE(LOG_TAG,"%s",text);
}
void Java_yon_AndroidGLView_nativeOnSurfaceDestroy(JNIEnv *pEnv, jobject obj){
}

