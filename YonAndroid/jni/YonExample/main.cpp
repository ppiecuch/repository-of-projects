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

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;

#pragma comment(lib, "Yon.lib")


#ifdef YON_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//YON_HIDE_CONSOLE


void print(const matrix4f& m){
	for(int i=0;i<4;++i)
		printf("%.2f\t%.2f\t%.2f\t%.2f\n",m.m[0][i],m.m[1][i],m.m[2][i],m.m[3][i]);
	printf("\n");
}

struct Object : public virtual core::IReferencable{
	int a;
	int b;
};

#define _CRTDBG_MAP_ALLOC
int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	//setlocale(LC_ALL,"chs");
	setlocale(LC_CTYPE,"UTF-8");

	

	SYonEngineParameters params;
	params.windowSize.w=400;
	params.windowSize.h=400;
	IYonEngine* engine=CreateEngine(params);

	IVideoDriver* driver=engine->getVideoDriver();
	ISceneManager* sceneMgr=engine->getSceneManager();
	IFileSystem* fs=engine->getFileSystem();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();

	ICamera* camera=sceneMgr->addCamera(core::vector3df(0,0,300));

	IMaterial* material;

	IEntity* cube=geometryFty->createCube(core::dimension3df(50,50,50));
	IModel* cubeModel=sceneMgr->addModel(cube);
	material=cubeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
	cubeModel->setPosition(core::vector3df(70,40,0));
	material->setTexture(0,driver->getTexture("../media/test.png"));
	cube->drop();

	//Logger->debug("%d\n",fs->existFile("D:/java1.exe"));

	//driver->getTexture("D:/test.png");
	//driver->getTexture("D:/test.png");

	//IImage* image=driver->createImageFromFile("D:/test.png");
	//image->drop();



	IEntity* sphere=geometryFty->createSphere(100,16,16);
	IModel* sphereModel=sceneMgr->addModel(sphere);
	material=sphereModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
	sphereModel->setPosition(core::vector3df(-50,-50,-150));
	material->setTexture(0,driver->getTexture("../media/earth.png"));
	sphere->drop();

	IEntity* plane=geometryFty->createXYPlane(core::dimension2df(50,50));
	IModel* planeModel=sceneMgr->addModel(plane);
	material=planeModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	planeModel->setPosition(core::vector3df(-50,20,0));
	material->setTexture(0,driver->getTexture("../media/aura.png"));
	plane->drop();

	IEntity* nav=geometryFty->createXYPlane(core::dimension2df(128,128));
	IModel* navModel=sceneMgr->addModel(nav);
	material=navModel->getMaterial(0);
	material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
	navModel->setPosition(core::vector3df(-100,-100,0));
	material->setTexture(0,driver->getTexture("../media/nav.png"));
	nav->drop();


	/*ILogger* logger=Logger;
	logger->setAppender(MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE|MASK_APPENDER_VS);
	//logger->setAppender(MASK_APPENDER_CONSOLE);
	logger->setLevel(ENUM_LOG_LEVEL_DEBUG);
	int num=0;*/
	f32 factor=1.001f;
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
	DEFAULT_MATERIAL->setMaterialType(ENUM_MATERIAL_TYPE_LIGHTEN);
	while(engine->run()){

		//const core::vector3df pos=camera->getPosition();
		//camera->setPosition(core::vector3df(pos.x,pos.y+0.005f ,pos.z));

		const core::vector3df crot=cubeModel->getRotation();
		cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.05f ,crot.z));

		const core::vector3df srot=sphereModel->getRotation();
		sphereModel->setRotation(core::vector3df(srot.x,srot.y-0.02f ,srot.z));

		const core::vector3df psca=planeModel->getScale();
		if(psca.x>4)
			factor=0.999f;
		else if(psca.x<2)
			factor=1.001f;
		planeModel->setScale(psca*factor);

		//const core::vector3df sca=model->getScale();
		//model->setScale(core::vector3df(sca.x+0.001f,sca.y+0.001f,sca.z+0.001f));

		driver->begin(true,COLOR_BLACK);

		sceneMgr->render(driver);

		driver->setMaterial(DEFAULT_MATERIAL);
		//driver->draw2DImage(tex,ORIGIN_POSITION2DI,core::recti(ORIGIN_POSITION2DI,tex->getSize()),NULL,COLOR_BLACK);

		Logger->drawString(core::stringc(" !#01234ABCDhelloworld"));

		//driver->endScene();
		//Sleep(20);
		//printf("run\n");
		//num++;
		//Logger->debug("%s\n","test你好");
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
	//效率与irrlicht相当
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//效率比irrlicht差1/3
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		array<u32> arr(12);
		array<u32> brr(13);
		arr=brr;
	}
	Logger->info("end\n");
#endif
#if 0
	//10万级别,1分20秒
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<100000;++i){
		arr.insert(1,0);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
#if 0
	//千万级,2秒,重新分配50次
	array<u32> arr;
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.push(1);
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");

	//千万级,1.5秒,重新分配50次
	Logger->info("start\n");
	for(u32 i=0;i<10000000;++i){
		arr.pop();
	}
	Logger->info("size:%d\n",arr.size());
	Logger->info("end\n");
#endif
	engine->drop();

	
#if 0
	//setlocale(LC_ALL,"zh_CN.utf8");
	setlocale(LC_ALL,"chs");

	core::stringc str;
	printf("%d,%s\n",str.length(),str);

	str+="test";
	printf("%d,%s\n",str.length(),str);

	core::stringc str1=str+"test";
	printf("%d,%s\n",str1.length(),str1);

	core::stringw wstr(L"%s",L"天天向上");
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	wstr+=L"中华人民共和国";
	wprintf(L"%d,%s\n",wstr.length(),wstr);

	core::stringw wstr1=wstr+L"test";
	wprintf(L"%d,%s\n",wstr1.length(),wstr1);

	core::stringc tstr("你好");
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