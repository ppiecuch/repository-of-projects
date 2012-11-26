#include "stdio.h"
#include<windows.h>

#include <crtdbg.h>
#include <typeinfo>

//#include <vld.h>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

//#ifdef YON_HIDE_CONSOLE
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
//#endif//YON_HIDE_CONSOLE

#include "framework.h"

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	init(NULL,NULL,NULL,800,480);
	while(getEngine()->run()){
		drawFrame();
	}
	destroy();
}

#if 0

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;
using namespace yon::scene::animator;
using namespace yon::audio;

#pragma comment(lib, "Yon.lib")



void print(const matrix4f& m){
	for(int i=0;i<4;++i)
		printf("%.2f\t%.2f\t%.2f\t%.2f\n",m.m[0][i],m.m[1][i],m.m[2][i],m.m[3][i]);
	printf("\n");
}

struct Object : public virtual core::IReferencable{
	int a;
	int b;
};

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				printf("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				printf("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			}
		}
	}
};

#define _CRTDBG_MAP_ALLOC
SYonEngineParameters params;
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	

	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pEventReceiver=new MyEventReceiver();
	IYonEngine* engine=CreateEngine(params);

	IVideoDriver* driver=engine->getVideoDriver();
	IAudioDriver* audioDriver=engine->getAudioDriver();
	ISceneManager* sceneMgr=engine->getSceneManager();
	IGraphicsAdapter* gfAdapter=engine->getGraphicsAdapter();
	IFileSystem* fs=engine->getFileSystem();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	ITimer* timer=engine->getTimer();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();

	ISound* sound=audioDriver->getSound("../media/bg.ogg");
	sound->setLooping(true);
	sound->setGain(0.5f);
	sound->play();
	sound=audioDriver->getSound("../media/helloworld.wav");
	sound->play();
	

	//Logger->setAppender(MASK_APPENDER_FILE|MASK_APPENDER_VS);
	Logger->debug("time:%d\n",timer->getTime());

	ICamera* camera=sceneMgr->addCamera(core::vector3df(0,0,300));

	IMaterial* material;
	IShap *shap,*shap1,*shap2;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createCube(50,50,50);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	IModel* cubeModel=sceneMgr->addModel(entity);
	material=cubeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	cubeModel->setPosition(core::vector3df(50,50,200));
	material->setTexture(0,driver->getTexture("../media/test.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("D:/kemulator.cfg",io::ENUM_ENDIAN_MODE_BIG);
	Logger->debug("read:%c\n",rs->readByte());
	Logger->debug("read:%04x\n",rs->readInt());
	rs->drop();

	map<s32,c8> m;
	m.insert(4,'4');
	m.insert(6,'6');
	m.insert(9,'9');
	m.remove(6);
	m.remove(5);
	m.remove(4);
	Logger->debug("map.size:%d\n",m.size());
	map<s32,c8>::Iterator it=m.getIterator();
	for(;it.atEnd()==false;++it)
		Logger->debug("%c\n",it->getValue());
	m.clear();

	map<s32,s32*> mm;
	mm.insert(4,new s32(4));
	map<s32,s32*>::Iterator mit=mm.getIterator();
	for(;mit.atEnd()==false;mit++)
		delete mit->getValue();
	mm.clear();

	

	list<u32> l;
	l.push_back(1);
	l.push_back(3);
	l.insert_after(l.begin(),2);

	//Logger->debug("%d\n",fs->existFile("D:/java1.exe"));

	//driver->getTexture("D:/test.png");
	//driver->getTexture("D:/test.png");

	//IImage* image=driver->createImageFromFile("D:/test.png");
	//image->drop();

	/*shap=geometryFty->createSphere(80,64,64);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	IModel* sphereModel=sceneMgr->addModel(entity);
	material=sphereModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	//material->setPolygonMode(ENUM_POLYGON_MODE_LINE);
	sphereModel->setPosition(core::vector3df(100,100,0));
	material->setTexture(0,driver->getTexture("../media/earth.png"));
	shap->drop();
	unit->drop();
	entity->drop();
	

	shap=geometryFty->createTorus(10,30,16,16,COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	IModel* toruseModel=sceneMgr->addModel(entity);
	material=toruseModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	material->setPolygonMode(ENUM_POLYGON_MODE_FILL);
	toruseModel->setPosition(core::vector3df(130,130,100));
	material->setTexture(0,driver->getTexture("../media/gunny.png"));
	shap->drop();
	unit->drop();
	entity->drop();*/

	

	/*IEntity* plane=geometryFty->createXYPlane(core::dimension2df(50,50));
	IModel* planeModel=sceneMgr->addModel(plane);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(-50,20,0));
	material->setTexture(0,driver->getTexture("../media/aura.png"));
	plane->drop();*/

	shap=geometryFty->createXYRectangle2D(-25,-25,25,25);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
	entity=geometryFty->createEntity(unit);
	IModel* planeModel=sceneMgr->addModel(entity);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(150,120,0));
	material->setTexture(0,driver->getTexture("../media/aura.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	/*IEntity* nav=geometryFty->createXYPlane(core::dimension2df(128,128));
	IModel* navModel=sceneMgr->addModel(nav);
	material=navModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	navModel->setPosition(core::vector3df(-100,-100,0));
	material->setTexture(0,driver->getTexture("../media/nav.png"));
	nav->drop();*/

	
	/*shap1=geometryFty->createXYRectangle(0,0,128,128,0,0,1,1);
	shap2=geometryFty->createXYRectangle(128,0,256,128,0,0,1,1);
	shap1->append(shap2);
	shap2->drop();
	unit=geometryFty->createUnit(shap1);
	IEntity* nav=geometryFty->createEntity(unit);
	IModel* navModel=sceneMgr->addModel(nav);
	material=navModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	navModel->setPosition(core::vector3df(100,100,0));
	material->setTexture(0,driver->getTexture("../media/nav.png"));
	//material->setTexture(0,Logger->getDebugPrinter()->getTexture());
	nav->drop();
	unit->drop();
	shap1->drop();*/

	

	/*shap=geometryFty->createXYRectangle2D2T(-25,-50,25,50,0,0,1,0.1f);
	unit=geometryFty->createUnit(shap);
	unit->setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
	entity=geometryFty->createEntity(unit);
	IModel* waterfallModel=sceneMgr->addModel(entity);
	material=waterfallModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_MASK);
	waterfallModel->setPosition(core::vector3df(90,100,120));
	material->setTexture(0,driver->getTexture("../media/waterfall.png"));
	material->setTexture(1,driver->getTexture("../media/mask.png"));
	shap->drop();
	unit->drop();
	entity->drop();

	SAnimatorParam aniParam;
	aniParam.type=ENUM_ANIMATOR_TYPE_UV;
	aniParam.animatorUV.unitIndex=0;
	aniParam.animatorUV.stage=0;
	IAnimator* uvAnimator=animatorFty->createAnimator(aniParam);
	waterfallModel->addAnimator(uvAnimator);
	uvAnimator->drop();*/


	/*ILogger* logger=Logger;
	logger->setAppender(MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE|MASK_APPENDER_VS);
	//logger->setAppender(MASK_APPENDER_CONSOLE);
	logger->setLevel(ENUM_LOG_LEVEL_DEBUG);
	int num=0;*/
	f32 factor=1.01f;
	ITexture* tex=driver->getTexture("../media/firework.png");
	DEFAULT_MATERIAL->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);

	/*IReadFile* t=fs->createAndOpenFile("../media/debug_font.png");
	long size=t->getSize();
	byte* buffer=new byte[size];
	t->read(buffer,size);
	Logger->setFormat(MASK_FORMAT_LOG);
	for(u32 i=0;i<(u32)size;++i)
		Logger->debug("%d,",buffer[i]);
	Logger->debug("\n");
	delete[] buffer;
	t->drop();*/
	/*IImage* t=driver->createImageFromFile("../media/debug_font.png");
	const dimension2du d=t->getDimension();
	u32 count=d.h*t->getByteCountPerRow();
	u8* buffer=(u8*)t->lock();
	Logger->setFormat(MASK_FORMAT_LOG);
	for(u32 i=0;i<count;++i)
	{
		if(i%40==0)Logger->debug("\n");
		Logger->debug("%d,",buffer[i]);
	}
	Logger->debug("\n");
	t->drop();*/
	//DEFAULT_MATERIAL->setTexture(0,tex);
	//DEFAULT_MATERIAL->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	while(engine->run()){

		//const core::vector3df pos=camera->getPosition();
		//camera->setPosition(core::vector3df(pos.x,pos.y+0.005f ,pos.z));

		const core::vector3df crot=cubeModel->getRotation();
		cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

		//const core::vector3df srot=sphereModel->getRotation();
		//sphereModel->setRotation(core::vector3df(srot.x,srot.y-0.8f ,srot.z));

		const core::vector3df psca=planeModel->getScale();
		if(psca.x>4)
			factor=0.99f;
		else if(psca.x<2)
			factor=1.01f;
		planeModel->setScale(psca*factor);

		//const core::vector3df trot=toruseModel->getRotation();
		//toruseModel->setRotation(core::vector3df(trot.x+1.0f,trot.y,trot.z));

		//const core::vector3df sca=model->getScale();
		//model->setScale(core::vector3df(sca.x+0.001f,sca.y+0.001f,sca.z+0.001f));

		driver->begin(true,COLOR_BLACK);

		sceneMgr->render(driver);

		//driver->setMaterial(DEFAULT_MATERIAL);
		//driver->draw2DImage(tex,ORIGIN_POSITION2DI,core::recti(ORIGIN_POSITION2DI,tex->getSize()),NULL,COLOR_BLACK);

		/*gfAdapter->beginBatch(0);
		gfAdapter->drawImage("../media/nav.png",0,0,128,128,0,0,true);
		gfAdapter->drawImage("../media/nav.png",0,0,128,128,100,0,true);
		gfAdapter->endBatch();*/

		Logger->drawString(core::stringc("FPS:%d",driver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

		//Logger->render();
		//driver->endScene();
		//Sleep(20);
		//printf("run\n");
		//num++;
		//Logger->debug("%s\n","test���");
		//logger->info("TEST\n");
		driver->end();
	}
#if 0
	IEntity* cube=geometryFty->createCube();
	cube->drop();
#endif 
#if 0
	Object* o=new Object();
	list<Object*> l;
	l.push_back(o);
	o->drop();
	l.clear();
#endif

#if 0
	array<u32> arr(12);
	array<u32> brr(13);
	//Ч����irrlicht�൱
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//Ч�ʱ�irrlicht��1/3
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		array<u32> arr(12);
		array<u32> brr(13);
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//10�򼶱�,1��20��
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<100000;++i){
		arr.insert(1,0);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
#if 0
	//ǧ��,2��,���·���50��
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.push(1);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");

	//ǧ��,1.5��,���·���50��
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.pop();
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
	engine->drop();
	delete params.pEventReceiver;
	
#if 0
	//setlocale(LC_ALL,"zh_CN.utf8");
	setlocale(LC_ALL,"chs");

	core::stringc str;
	printf("%d,%s\n",str.length(),str);

	str+="test";
	printf("%d,%s\n",str.length(),str);

	core::stringc str1=str+"test";
	printf("%d,%s\n",str1.length(),str1);

	core::stringw wstr(L"%s",L"��������");
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	wstr+=L"�л����񹲺͹�";
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	core::stringw wstr1=wstr+L"test";
	wprintf(L"%d,%s\n",wstr1.length(),wstr1);

	core::stringc tstr("���");
	printf("%s\n\n\n",tstr);

	printf("%s\n",(tstr+"1").c_str());
#endif

#if 0
	matrix4f m;
	m.makeIdentity();
	print(m);

	//vector3df v(3,2,3);
	m.setTranslation(3,2,3);
	print(m);

	matrix4f n;
	n.makeIdentity();
	n.translate(1,2,3);
	print(n);

	matrix4f r;
	r=m*n;
	print(r);

	f32 temp[]={2,0,0,1,0,1,0,2,0,0,1,3,0,0,0,1};
	matrix4f l(temp);
	r=m*l;
	printf("==>");
	print(r);

	print(IDENTITY_MATRIX);

	l.makeInverse();
	print(l);

	//r.perspective(60,2,1,100);
	//r.ortho(-5,5,-3,3,1,100);
	r.lookAt(1,2,3,6,5,4,0,1,0);
	//r.makeIdentity();
	//r.scale(1,2,3);
	print(r);

	r.rotate(60,1,1,1);
	print(r);

	f32 temp2[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	matrix4f k(temp2);
	print(k);
	k.makeTranspose();
	print(k);

	
#endif

	//getchar();
	return 0;
}

#endif