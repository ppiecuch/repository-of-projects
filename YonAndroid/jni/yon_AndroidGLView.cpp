#include "yon_AndroidGLView.h"
#include "include/yon.h"
#include "log.h"
#include <GLES/gl.h>
#include <locale.h>

using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::io;
using namespace yon::debug;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;

IYonEngine* engine=NULL;
IVideoDriver* driver=NULL;
ISceneManager* sceneMgr=NULL;

IModel* cubeModel=NULL;
IModel* sphereModel=NULL;
IModel* planeModel=NULL;

const static s32 ACTION_MASK = 255;
const static s32 ACTION_DOWN = 0;
const static s32 ACTION_UP = 1;
const static s32 ACTION_MOVE = 2;
const static s32 ACTION_POINTER_DOWN = 5;
const static s32 ACTION_POINTER_UP = 6;

const char* LOG_TAG = "yon_AndroidGLView";

void Java_yon_AndroidGLView_nativeOnSurfaceCreated(JNIEnv *pEnv, jobject obj, jstring apkFilePath, jstring sdcardPath){
	LOGD(LOG_TAG,"pEnv:%08x,nativeOnSurfaceCreated",pEnv);
	SYonEngineParameters params;
	params.pJNIEnv=pEnv;
	engine=CreateEngine(params);

	driver=engine->getVideoDriver();
	sceneMgr=engine->getSceneManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IFileSystem* fs=engine->getFileSystem();
	ICamera* camera=sceneMgr->addCamera(core::vector3df(0,0,300));

	IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

	/*IEntity* cube=geometryFty->createCube(core::dimension3df(50,50,50));
	cubeModel=sceneMgr->addModel(cube);
	//material=cubeModel->getMaterial(0);
	cubeModel->setPosition(core::vector3df(20,20,0));
	//material->setTexture(0,driver->getTexture("/media/test.png"));
	cube->drop();*/
	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	cubeModel->setPosition(core::vector3df(70,40,0));
	material->setTexture(0,driver->getTexture("/media/test.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	/*IEntity* sphere=geometryFty->createSphere(50,16,16);
	sphereModel=sceneMgr->addModel(sphere);
	//material=sphereModel->getMaterial(0);
	sphereModel->setPosition(core::vector3df(-20,-20,0));
	//material->setTexture(0,driver->getTexture("/media/earth.png"));
	sphere->drop();*/
	shap=geometryFty->createSphere(100,16,16);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	sphereModel=sceneMgr->addModel(entity);
	material=sphereModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	sphereModel->setPosition(core::vector3df(-50,-50,-150));
	material->setTexture(0,driver->getTexture("/media/earth.png"));
	shap->drop();
	unit->drop();
	entity->drop();
	
	/*IEntity* plane=geometryFty->createXYPlane(core::dimension2df(50,50));
	planeModel=sceneMgr->addModel(plane);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(-20,20,100));
	material->setTexture(0,driver->getTexture("/media/aura.png"));
	plane->drop();*/
	shap=geometryFty->createXYRectangle(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(-50,20,0));
	material->setTexture(0,driver->getTexture("/media/aura.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	shap1=geometryFty->createXYRectangle(0,0,128,128,0,0,1,1);
	shap2=geometryFty->createXYRectangle(128,0,256,128,0,0,1,1);
	shap1->append(shap2);
	shap2->drop();
	unit=geometryFty->createUnit(shap1);
	IEntity* nav=geometryFty->createEntity(unit);
	IModel* navModel=sceneMgr->addModel(nav);
	material=navModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	navModel->setPosition(core::vector3df(-100,-100,0));
	material->setTexture(0,driver->getTexture("/media/nav.png"));
	//material->setTexture(0,Logger->getDebugPrinter()->getTexture());
	nav->drop();
	unit->drop();
	shap1->drop();

	LOGD(LOG_TAG,"nativeOnSurfaceCreated");
}
void Java_yon_AndroidGLView_nativeOnSurfaceChanged(JNIEnv *pEnv, jobject obj, jint w, jint h){
	LOGD(LOG_TAG,"nativeOnSurfaceChanged->w:%d,h:%d",w,h);
	engine->onResize(w,h);
}
f32 factor=1.01f;
void Java_yon_AndroidGLView_nativeOnDrawFrame(JNIEnv *pEnv, jobject obj){
	//LOGD(LOG_TAG,"nativeOnDrawFrame");
	//glClearColor(0.1f,0.2f,0.3f,1);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	engine->run();
	driver->begin(true,COLOR_BLACK);

	const core::vector3df crot=cubeModel->getRotation();
	cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	const core::vector3df srot=sphereModel->getRotation();
	sphereModel->setRotation(core::vector3df(srot.x,srot.y-0.5f ,srot.z));

	const core::vector3df psca=planeModel->getScale();
	if(psca.x>4)
		factor= 0.99f;
	else if(psca.x<2)
		factor=1.01f;
	planeModel->setScale(psca*factor);

	sceneMgr->render(driver);

	Logger->drawString(core::stringc("FPS:%d",driver->getFPS()),ORIGIN_POSITION2DI,COLOR_GREEN);
	Logger->render();

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
	engine->postEventFromUser(evt);
	return false;
}
void Java_yon_AndroidGLView_nativeOnSurfaceDestroy(JNIEnv *pEnv, jobject obj){
	LOGD(LOG_TAG,"nativeOnSurfaceDestroy");
}