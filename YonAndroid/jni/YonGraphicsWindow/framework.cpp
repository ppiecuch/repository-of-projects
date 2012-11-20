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
IRandomizer* randomizer=NULL;
ITimer* timer=NULL;

IModel* cubeModel=NULL;
IModel* planeModel=NULL;
f32 factor=1.1f;

#include "SDynamicShap.h"

core::array<core::stringc> images;
core::array<ITexture*> textures;
scene::SDynamicShap3D shaps[29];
SMaterial material;

class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
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
				return true;
			}
		}
		return false;
	}
};

bool init(void *pJNIEnv,ICallback* pcb,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapterWindow();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO_WINDOW,NULL,core::vector3df(0,0,-300),core::vector3df(0,-1,0)); 
	logger=Logger;
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	//fs->addWorkingDirectory("../media/batch");
#elif defined(YON_COMPILE_WITH_ANDROID)
	//fs->addWorkingDirectory("media/");
	fs->addWorkingDirectory("media/batch");
#endif

	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	/*shap=geometryFty->createXYRectangle2D(-125,-125,125,125);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	planeModel=sceneMgr->addModel(entity);
	SMaterial& material=planeModel->getMaterial(0);
	material.MaterialType=ENUM_MATERIAL_TYPE_BLEND;
	material.BlendSrc=ENUM_BLEND_FACTOR_SRC_ALPHA;
	material.BlendDst=ENUM_BLEND_FACTOR_ONE;
	material.setTexture(0,videoDriver->getTexture("aura.png"));
	planeModel->setPosition(core::vector3df(0,0,0));
	shap->drop();
	unit->drop();
	entity->drop();*/

	for(u32 i=1;i<=7;++i){
		images.push_back(core::stringc("%d.png",i));
		textures.push_back(videoDriver->getTexture(images[i-1]));
	}

	s32 z=900;
	u16 index=0;
	for(u32 i=0;i<29;++i){
		shaps[i].getVertexArray().set_used(0);
		shaps[i].getIndexArray().set_used(0);
		//shaps[i].setIndexHardwareBufferUsageType(ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		//shaps[i].setVertexHardwareBufferUsageType(ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		for(u32 j=0;j<35;++j)
		{
			s32 x=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().w);
			s32 y=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().h);
			shaps[i].getVertexArray().push_back(SVertex(x,y,z,0,0,COLOR_WHITE));
			shaps[i].getVertexArray().push_back(SVertex(x+64,y,z,1,0,COLOR_WHITE));
			shaps[i].getVertexArray().push_back(SVertex(x+64,y+32,z,1,1,COLOR_WHITE));
			shaps[i].getVertexArray().push_back(SVertex(x,y+32,z,0,1,COLOR_WHITE));
			shaps[i].getIndexArray().push_back(0+index);
			shaps[i].getIndexArray().push_back(1+index);
			shaps[i].getIndexArray().push_back(3+index);
			shaps[i].getIndexArray().push_back(3+index);
			shaps[i].getIndexArray().push_back(1+index);
			shaps[i].getIndexArray().push_back(2+index);
			index+=4;
		}
		--z;
		index=0;
	}

	ITexture* texture=textures[0];
	material.setTexture(0,texture);

	
	material.FrontFace=ENUM_FRONT_FACE_CW;

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
//#define BATCH
void drawFrame(){

	videoDriver->begin(true,true,COLOR_DEFAULT);

	//pCamera->setPosition(core::vector3df(x,x,300),true);

	sceneMgr->render(videoDriver);
	//pCamera->render(videoDriver);

#ifndef BATCH
	gfAdapter->clearZ(1000);
#endif
	

	static core::rectf r(0,1,1,0);
	videoDriver->setMaterial(material);

	//LG990
	//1000-adapter-drawRegion-不开启VBO-使用batch=250ms
	//1000-adapter-drawRegion-不开启VBO-不使用batch=270ms
	//29-driver-drawShap-开启VBO=140ms
	//29-driver-drawShap-不开启VBO=120ms
	//29-driver-drawShap-不开启VBO-不切换纹理=120ms
	for(u32 i=0;i<1;++i){
		//gfAdapter->drawRegion("shadow.png",r,randomizer->rand(0,400),randomizer->rand(0,400),128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER),true,0xFF0000FF);
		//gfAdapter->drawRegion(images[randomizer->rand(1,images.size()-1)].c_str(),r,randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().w),randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().h),128,64,ENUM_TRANS_NONE);

#ifndef BATCH
		//ITexture* texture=textures[randomizer->rand(0,textures.size()-1)];
		//s32 x=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().w);
		//s32 y=randomizer->rand(0,videoDriver->getCurrentRenderTargetSize().h);
		//gfAdapter->drawRegion(texture,r,x,y,128,64,ENUM_TRANS_NONE);
		ITexture* texture=videoDriver->getTexture("de.png");
		gfAdapter->drawRegion(texture,r,0,0,512,256,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),false,0xFF0000FF);
		gfAdapter->drawRegion(texture,r,0,0,512,256,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),true);
#else
		//Logger->debug("i:%d--0x%08X\r\n",i,&shaps[i]);
		//material.setTexture(0,texture);
		//videoDriver->setMaterial(material);
		videoDriver->drawShap(&shaps[i]);
#endif

		//gfAdapter->drawRegion(images[randomizer->rand(1,images.size()-1)].c_str(),r,randomizer->rand(0,400),randomizer->rand(0,400),128,64,ENUM_TRANS_NONE);
		//gfAdapter->drawRegion("trans.png",r,100,120,128,64,ENUM_TRANS_ROT180);
		//gfAdapter->drawRegion("test.png",r,randomizer->rand(0,400),randomizer->rand(0,400),64,64,ENUM_TRANS_ROT90);
		/*gfAdapter->drawRegion("trans.png",r,200,120,128,64,ENUM_TRANS_MIRROR);
		gfAdapter->drawRegion("trans.png",r,300,120,128,64,ENUM_TRANS_MIRROR_ROT180);
		gfAdapter->drawRegion("shadow.png",r,50,170,128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER),true);
		gfAdapter->drawRegion("trans.png",r,100,30,128,64,ENUM_TRANS_MIRROR_ROT90);
		gfAdapter->drawRegion("trans.png",r,200,320,128,64,ENUM_TRANS_MIRROR_ROT270,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER));
		gfAdapter->drawRegion("trans.png",r,300,320,128,64,ENUM_TRANS_ROT270,(MASK_ACTHOR)(MASK_ACTHOR_RIGHT|MASK_ACTHOR_BOTTOM));*/
	}

	/*gfAdapter->drawVertexPrimitiveList(planeModel->getMaterial(0),
	planeModel->getEntity()->getUnit(0)->getShap()->getVertices(),
	planeModel->getEntity()->getUnit(0)->getShap()->getVertexCount(),
	planeModel->getEntity()->getUnit(0)->getShap()->getIndices(),
	planeModel->getEntity()->getUnit(0)->getShap()->getIndexCount(),
	planeModel->getEntity()->getUnit(0)->getShap()->getVertexType());*/

#ifndef BATCH
	gfAdapter->render();
#endif
	static u32 start,end,diff;

	//Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,use:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),diff),core::ORIGIN_POSITION2DI,COLOR_GREEN);
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,DCL:%d,use:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall(),diff),core::position2di(0,10),COLOR_GREEN);

	videoDriver->end();
	end=timer->getRealTime();
	diff=end-start;
	start=timer->getRealTime();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}