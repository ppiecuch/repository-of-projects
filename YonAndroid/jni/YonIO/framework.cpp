#include "framework.h"
SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
//IRandomizer* randomizer=NULL;
ITextSystem* textSystem=NULL;
II18NManager* i18nManager=NULL;
ITimer* timer=NULL;

SMaterial pointMaterial;
SDynamicShap2D pointShap;

ISceneNode* cubeModel=NULL;
ISceneNode* weedModel=NULL;
ISceneNode* planeModel=NULL;
ISceneNode* teapotModel=NULL;
f32 factor=1.1f;

core::matrix4f proj;
core::matrix4f view;

scene::SDynamicShap3D shap;
video::SMaterial material;

//编译时间
core::stringc getVersion(){
	return core::stringc("%s %s",__DATE__,__TIME__);
}


s32 strlen_utf8(const c8 *s) {
	s32 i = 0, j = 0;
	while (s[i]) {
		if ((s[i] & 0xc0) != 0x80) j++;
		i++;
	}
	return j;
}

/* 
void substr (char *dest,char *str, unsigned start, unsigned end)
{
unsigned n = end - start;
printf("\n%u \n",n);
strncpy(dest, str + start, n);
dest[n] = 0;
}
int Pattern(char *str,char *pattern,char *result)
{
int cflags=REG_EXTENDED,z;
regex_t reg;
regmatch_t pm[10];
const size_t nmatch=10;
z = regcomp (&reg, pattern, cflags);
printf("%d\n",z);
if(z!=0) return z;
z = regexec (&reg, str, nmatch, pm, 0);
printf("%d\n",z);
if(z!=0) return z;
substr(result,str, pm[0].rm_so, pm[0].rm_eo);
return z;
}
*/
class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt)
	{
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				event::SEvent ev;
				ev.type=event::ENUM_EVENT_TYPE_SYSTEM;
				ev.systemInput.type=event::ENUM_SYSTEM_INPUT_TYPE_EXIT;
				getEngine()->postEventFromUser(ev);
				return true;
			}
		}
		return false;
	}
};
class App
{
public:
	typedef CDelegate1<void, const core::stringc&> Callback;

	void debug(const core::stringc& str)
	{
		YON_DEBUG("App:%s\r\n",str.c_str());
	}

	static void info(const core::stringc& str)
	{
		YON_INFO("App:%s\r\n",str.c_str());
	}
};
class Rt
{
public:
	typedef CDelegate1<void, const core::stringc&> Callback;

	void debug(const core::stringc& str)
	{
		YON_DEBUG("Rt:%s\r\n",str.c_str());
	}

	static void info(const core::stringc& str)
	{
		YON_INFO("Rt:%s\r\n",str.c_str());
	}
};
bool init(void *pJNIEnv,ICallback* pcb,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.pCallback=pcb;
	//params.fpsLimit=10;
	params.appPath=appPath;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	i18nManager=engine->getI18NManager();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	textSystem=engine->getTextSystem();
	fs=engine->getFileSystem();
	//pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,NULL,core::vector3df(0,0,300));
	logger=Logger;
	timer=engine->getTimer();
	//randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/",false);
	//fs->addWorkingDirectory("../Yon/");
	//fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/",true);
#endif

	//Logger->debug("%s\n",fs->getAbsolutePath(fs->getWorkingDirectory()).c_str());

	/*pointMaterial.PolygonMode=video::ENUM_POLYGON_MODE_POINT;

	core::array<S2DVertex>& vertices=pointShap.getVertexArray();
	core::array<u16>& indices=pointShap.getIndexArray();

	S2DVertex v1(100,100,0,0,video::COLOR_WHITE);
	S2DVertex v2(100,150,0,0,video::COLOR_WHITE);
	S2DVertex v3(150,100,0,0,video::COLOR_WHITE);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);*/

#if 0
	YON_DEBUG("ver:%s\r\n",getVersion().c_str());
#elif 0
	float f=300.56f;
	float tmp1=f;
	unsigned int* i=(unsigned int*)&tmp1;
	float tmp2=f;
	unsigned int* l=(unsigned int*)&tmp2;
	*l = (*l << 24) | ((*l & 0xFF00)  << 8) | ((*l & 0x00FF0000) >> 8) | (*l >> 24);
	YON_DEBUG("f:%.2f,%08X,%08X\r\n",f,*i,*l);
#elif 0
	f32 Near=1000;
	f32 Far=20000;
	f32 Fovy=core::PI/17;
	f32 Aspect=1024/768.f;

	f32 frustumW, frustumH;
	frustumH = tanf(Fovy/2) * Near;
	frustumW = frustumH * Aspect;
	YON_DEBUG("Fovy/2:%.5f,frustumH:%.5f,frustumW:%.5f\r\n",Fovy/2,frustumH,frustumW);

	Near=1000;
	Far=20000;
	Aspect=768/1024.f;
	Fovy=atan(tanf(Fovy/2)/Aspect)*2;
	frustumH = tanf(Fovy/2) * Near;
	frustumW = frustumH * Aspect;
	YON_DEBUG("Fovy/2:%.5f,frustumH:%.5f,frustumW:%.5f\r\n",Fovy/2,frustumH,frustumW);
#elif 0
	int w=480;
	int h=320;
	core::matrix4f m1;
	m1.makeIdentity();
	m1.lookAt(480,320,553,480,320,0,0,1,0);
	m1.print();
	m1.translate(w,h,0);
	m1.rotate(-90,0,0,1);
	m1.translate(-h,-w,0);
	m1.print();
	m1.makeIdentity();
	m1.translate(160,240,0);
	m1.print();
	m1.rotate(-90,0,0,1);
	m1.print();
	m1.translate(-240,-160,0);
	m1.print();
	m1.makeIdentity();
	m1.rotate(-90,0,0,1);
	m1.ortho(-240.00f,240.00f,-160.00f,160.00f,1,3000);
	m1.print();
	core::matrix4f proj;
	proj.makeIdentity();
	proj.perspective(60.f*core::DEGTORAD,0.667f,0.500f,1500.000f);
	proj.print();
	proj.makeIdentity();
	proj.perspective(60.f*core::DEGTORAD,1/0.667f,0.500f,1500.000f);
	proj.print();
	//1.000f,0.000f,0.000f,0.000f,
	//0.000f,1.000f,0.000f,0.000f,
	//0.000f,0.000f,1.000f,0.000f,
	//0.000f,0.000f,-300.000f,1.000f
	core::matrix4f view;
	view.makeIdentity();
	//view.lookAt(160.000f,240.000f,415.010f,160.000f,240.000f,0.000f,0,1,0);
	view.lookAt(0,0,-300,0,0,0,0,1,0);
	view.print();

	//view.translate(160, 240, 0);
	//view.rotate(-90, 0, 0, 1);
	//view.translate(-160, -240, 0);
	view.translate(160, 240, 0);
	view.rotate(-90, 0, 0, 1);
	view.translate(-240, -160, 0);
	view.print();
#elif 0
	//char result[100];
	//Pattern("Windows 95","Windows (?=95|98|NT|2000)",result);
	//printf("%s\n",result);
	IPattern* pattern=textSystem->compile("<title>(.*)</title>");
	IMatcher* matcher=pattern->matcher("111 <title>Hello World</title> 222");
	matcher->drop();
	pattern->drop();
#elif 0
	Logger->debug("%s\n",fs->getResourcePath("test.png").c_str());
	Logger->debug("%s\n",fs->getResourcePath("Android.mk").c_str());
	Logger->debug("%s\n",fs->getResourcePath("nothing.png").c_str());
	Logger->debug("%s\n",fs->getResourcePath("D:\\Development\\Software\\android-ndk-r7\\samples\\Irrlicht\\Examples\\nothing.png").c_str());

	Logger->debug("%s\n",fs->getAbsolutePath("test.png").c_str());
	Logger->debug("%s\n",fs->getAbsolutePath("D:\\Development\\Software\\android-ndk-r7\\samples\\Irrlicht\\Examples\\nothing.png").c_str());

	Logger->debug("%s\n",core::getParentName(fs->getResourcePath("tst/tst/tst/test.txt",true)).c_str());
	//Logger->debug("%d\n",_mkdir("d:/Development/Software/android-ndk-r7/samples/YonAndroid/jni/media/tst/tst/tst/"));
	//core::mkdirs(io::path("d:/Development/Software/android-ndk-r7/samples/YonAndroid/jni/media/tst/tst/tst/"));
	Logger->debug("success\n");

	//core::mkdirs(fs->getAbsolutePath(io::path("../media\\test1\\test2")).c_str());

	//Logger->debug("%s\n",core::getParentName(io::path("../media/fdsafs.txt")).c_str());

	core::ustring utf8_1("abcdefg");
	core::stringc origin_2=i18nManager->convert("你好UTF8",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	core::ustring utf8_2(origin_2.c_str());

	YON_DEBUG("%s(%d)\r\n",utf8_1.toStringc().c_str(),utf8_1.size());
	YON_DEBUG("%s(%d)\r\n",i18nManager->convert(utf8_2.toStringc().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str(),utf8_2.size());
#elif 0
	u32 test=0xFFFFFFFF;
	YON_DEBUG("%u,%08X\r\n",test,(s32)test);
#elif 1
	App app;
	Rt rt;

	void* p1=&App::info;
	void* p2=&Rt::info;

	//创建委托。
	App::Callback callback = NULL;
	if(!callback.empty()) callback("0");

	core::IDelegate1<void, const core::stringc&>* d0=core::createDelegate(&app,&App::debug);
	core::IDelegate1<void, const core::stringc&>* d1=core::createDelegate(&App::info);

	//d0->drop();
	//d1->drop();

	callback += d0;
	if(!callback.empty()) callback("1");

	callback += d1;
	if(!callback.empty()) callback("2");

	callback -= d0;
	if(!callback.empty()) callback("3");

	callback -= d1;
	if(!callback.empty()) callback("4");

	d0->clear();

	typedef core::CDelegate1<void, const core::stringc&> MulDG;
	MulDG callback2=NULL;
	if(!callback2.empty()) callback2("0");

	core::IDelegate1<void, const core::stringc&>* d2=core::createDelegate(&rt,&Rt::debug);
	core::IDelegate1<void, const core::stringc&>* d3=core::createDelegate(&Rt::info);

	//Rt::Callback test = NULL;
	//test+=d2;
	//test("Test");

	callback2 += d0;
	if(!callback2.empty()) callback2("A");

	callback2 += d1;
	if(!callback2.empty()) callback2("B");

	callback2 += d2;
	if(!callback2.empty()) callback2("C");

	callback2 += d3;
	if(!callback2.empty()) callback2("D");

	callback2 -= d0;
	if(!callback2.empty()) callback2("E");

	callback2 -= d1;
	if(!callback2.empty()) callback2("F");

	callback2 -= d2;
	if(!callback2.empty()) callback2("G");

	callback2 -= d3;
	if(!callback2.empty()) callback2("H");

	d0->drop();
	d1->drop();
	d2->drop();
	d3->drop();

#elif 0

	u32 start=timer->getRealTime();
	for(u32 i=0;i<1000000;++i)
		YON_DEBUG_BREAK_IF(nearestPowerOf22(i)!=core::nearestPowerOf2(i));

	YON_DEBUG("%u\r\n",(timer->getRealTime()-start));


#elif 0

	//s32 s;
	//core::convertor<core::stringc,s32>::convert(core::stringc("134FF2"),s);
	//YON_DEBUG("%d\r\n",s);

	video::SColor c1(0xFF134ff2);
	YON_DEBUG("%s\r\n",c1.toHexString(true,false).c_str());
	//YON_DEBUG("%s\r\n",toHexString(0xFF134ff2,false,false).c_str());

	video::SColor c=video::SColor::fromHexString("#123456FF");
	YON_DEBUG("%08X\r\n",c.color);


#elif 0
	f32 f;
	core::convertor<s32,f32>::convert(1,f);
	YON_DEBUG("%.2f\r\n",f);

#elif 0
	core::stringc& str=video::COLOR_RED.toHexString(true,false);
	YON_DEBUG("%s\r\n",str.c_str());

#elif 0
	IWriteStream* ws=fs->createAndOpenWriteFileStream("d:/test.txt");
	ws->writeBool(true);
	ws->writeFloat(2.1f);
	ws->writeLong(3333);
	ws->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("d:/test.txt");
	Logger->debug("%d\n",rs->readBool());
	Logger->debug("%.2f\n",rs->readFloat());
	Logger->debug("%ld\n",rs->readLong());
	rs->drop();
#elif 0
	IWriteStream* ws=fs->createAndOpenWriteFileStream("tst/tst/tst/test.txt",true,ENUM_ENDIAN_MODE_BIG);
	ws->writeBool(true);
	ws->writeFloat(2.1f);
	ws->writeLong(3333);
	ws->writeString("helloworld");
	ws->writeString(core::stringc("helloChina"));
	ws->drop();

	IReadStream* rs=fs->createAndOpenReadFileStream("tst/tst/tst/test.txt",ENUM_ENDIAN_MODE_BIG);
	Logger->debug("%d\n",rs->readBool());
	Logger->debug("%.2f\n",rs->readFloat());
	Logger->debug("%ld\n",rs->readLong());
	Logger->debug("%s\r\n",rs->readString().c_str());
	Logger->debug("%s\r\n",rs->readString().c_str());
	//Logger->debug("%d\n",rs->readBool());
	//Logger->debug("%.2f\n",rs->readFloat());
	//Logger->debug("%ld\n",rs->readLong());
	rs->drop();
#elif 0
	IReadStream* rs=fs->createAndOpenReadFileStream("rotate_x_.dae");
	XMLReader* reader=fs->createXMLReader(rs);
	rs->drop();

	while(reader->read());
	reader->drop();
#elif 1
	/*IReadStream* stream=fs->createAndOpenReadFileStream("consume.xlb",io::ENUM_ENDIAN_MODE_BIG);
	core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();

	Logger->debug("rowCount:%d\r\n",rowCount);

	Logger->debug("%d\r\n",stream->readInt());*/
	//Logger->debug("%s\r\n",UTF8ToGB18030(stream->readString().c_str()).c_str());
	//Logger->debug("%s\r\n",EncodeConvertor::getInstance().GB18030ToUTF8(stream->readString().c_str()).c_str());
#ifdef YON_COMPILE_WITH_WIN32
	//Logger->debug("%s\r\n",i18nManager->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	//Logger->debug("%s\r\n",i18nManager->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());

	
	const c8 candidate[]="￥�§¨фхцчшщъы";
	for(int i=0;i<strlen(candidate);++i)
	{
		core::stringc test=i18nManager->convert(core::stringc("%c",candidate[i]).c_str(),ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
		Logger->debug("%s\r\n",i18nManager->convert(test.c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	}
#elif defined(YON_COMPILE_WITH_ANDROID)
	//Logger->debug("%s\r\n",stream->readString().c_str());
	//Logger->debug("%s\r\n",stream->readString().c_str());
	core::stringc test2=i18nManager->convert("中华人民共和国",ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030);
	Logger->debug("%s\r\n",i18nManager->convert(test2.c_str(),ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8).c_str());
#endif
	//Logger->debug("%s\r\n",stream->readString().c_str());

	//stream->drop();
#elif 1
	/*c8 in_utf8[] = 
	{0x20,0xe8,0xbf,0x99,0xe6,0x98,0xaf,0xe4,0xb8,0x80,0xe4,0xb8,0xaa,0x69,0x63,0x6f,
	0x6e,0x76,0xe7,0x9a,0x84,0xe6,0xb5,0x8b,0xe8,0xaf,0x95,0xe4,0xbe,0x8b,0xe7,0xa8,
	0x8b,0xef,0xbc,0x9a,0x0a,0xe2,0x80,0x9c,0xe7,0xba,0xb5,0xe6,0xb5,0xb7,0xe6,0x9d,
	0xaf,0xe2,0x80,0x9d,0xe4,0xb8,0x9c,0xe5,0x8d,0x97,0xe5,0xa4,0xa7,0xe5,0xad,0xa6,
	0xe7,0xac,0xac,0xe4,0xb8,0x89,0xe5,0xb1,0x8a,0xe5,0xb5,0x8c,0xe5,0x85,0xa5,0xe5,
	0xbc,0x8f,0xe7,0xb3,0xbb,0xe7,0xbb,0x9f,0xe8,0xae,0xbe,0xe8,0xae,0xa1,0xe9,0x82,
	0x80,0xe8,0xaf,0xb7,0xe8,0xb5,0x9b,0x00};    //UTF-8编码*/
	//Logger->debug("%s\r\n",i18nManager->convert("中华人民共和国",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8).c_str());
	//Logger->debug("%s\r\n",i18nManager->convert("涓崕浜烘皯鍏卞拰鍥",ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
	//Logger->debug("%s\r\n",i18nManager->convert(in_utf8,ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());

	core::stringc str=i18nManager->convert("abc中华人民abc共和国abc",ENUM_ENCODING_GB18030,ENUM_ENCODING_UTF8);
	YON_DEBUG("len:%d,clen:%d\r\n",str.length(),strlen_utf8(str.c_str()));

#elif 0

	typedef core::map<s32,s32> TestMap;
	TestMap m;
	for(s32 i=0;i<100;++i)
		m.set(i,i);
	TestMap::Iterator iterator=m.getIterator();
	for(;!iterator.atEnd();++iterator)
		YON_DEBUG("%d->%d\r\n",iterator->getKey(),iterator->getValue());
#endif

	proj.makeIdentity();
	proj.perspective(60.f*core::DEGTORAD,0.667f,0.500f,1500.000f);

	view.makeIdentity();
	view.lookAt(160.000f,240.000f,415.010f,160.000f,240.000f,0.000f,0,1,0);
	view.makeTranspose();

	core::matrix4f m(0.000f,-1.000f,0.000f,0.000f,1.000f,-0.000f,0.000f,0.000f,0.000f,0.000f,1.000f,0.000f,-32.000f,32.000f,-415.010f,1.000f);
	view=m;

	proj.print();
	view.print();

	core::array<scene::SVertex>& vs=shap.getVertexArray();
	core::array<u16>& is=shap.getIndexArray();

	vs.push_back(scene::SVertex(0.000f,64.000f,0.000f,0,1,COLOR_WHITE));
	vs.push_back(scene::SVertex(0.000f,0.000f,0.000f,0,0,COLOR_WHITE));
	vs.push_back(scene::SVertex(64.000f,64.000f,0.000f,1,1,COLOR_WHITE));
	vs.push_back(scene::SVertex(64.000f,0.000f,0.000f,1,0,COLOR_WHITE));

	is.push_back(0);
	is.push_back(1);
	is.push_back(2);
	is.push_back(2);
	is.push_back(1);
	is.push_back(3);

	material.MaterialType=video::ENUM_MATERIAL_TYPE_TRANSPARENT;
	material.setTexture(0,videoDriver->getTexture("test.png"));
	material.CullingMode=video::ENUM_CULLING_MODE_NONE;

	return true; 
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	//const core::matrix4f& m=videoDriver->getTransform(video::ENUM_TRANSFORM_PROJECTION);
	//m.print();

	videoDriver->setMaterial(material);

	videoDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,proj);
	videoDriver->setTransform(video::ENUM_TRANSFORM_VIEW,view);

	videoDriver->drawShap(&shap);

	/*gfAdapter->clearZ(100);

	//core::rectf r(0,1,1,0);
	gfAdapter->drawImage("test.png",0,0,64,64,50,50);
	gfAdapter->render();*/


	//videoDriver->setMaterial(pointMaterial);
	//videoDriver->drawShap(&pointShap);

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}