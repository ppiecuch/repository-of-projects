#include "framework.h"
#include "BillBoardAmt.h"
#include "ParticleSystem.h"
#include "SpeEffectSet.h"
#include "ParticleEmiter.h"
SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
ILogger* logger=NULL;
ITimer* timer=NULL;
IRandomizer* randomizer=NULL;
int     lastTime=0;

IModel* cubeModel=NULL;
CBillBoardAmt * p_BillboardAmt=NULL;
CParticleSystem * p_ParticleSystem=NULL;
CSpeEffectSet  *  p_SpeEffectSet=NULL;

class MyEventListener : public IEventReceiver{
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
			switch(evt.touchInput.type)
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

bool init(void *pJNIEnv,u32 width,u32 height){
	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventListener();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300));
	logger=Logger;
	timer=engine->getTimer();
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

  
// 	p_BillboardAmt=new CBillBoardAmt();
// 	p_BillboardAmt->setPos(position2di(0,0));
// 	p_BillboardAmt->init();
// 	p_BillboardAmt->setDriver(videoDriver);
//     IReadStream * p_FileReader=engine->getFileSystem()->createAndOpenReadFileStream("exportBillBoard/5.bil");
// 	p_BillboardAmt->load(p_FileReader);
// 	delete p_FileReader;
// 	p_BillboardAmt->setPos(position2di(0,0));
	//p_BillboardAmt->setTexture(videoDriver->getTexture("portal1.bmp"));
	//p_BillboardAmt->setTexture(videoDriver->getTexture("aura.png"));
	//p_BillboardAmt->SetMixType(ENUM_BLEND_FACTOR_SRC_ALPHA,ENUM_BLEND_FACTOR_ONE);
	//p_BillboardAmt->SetAlphaSource(ENUM_ALPHA_SOURCE_TEXTURE);

   
    	/*

		enum ENUM_BLEND_FACTOR{
		ENUM_BLEND_FACTOR_ZERO	= 0,				//!< src & dest	(0, 0, 0, 0)
		ENUM_BLEND_FACTOR_ONE,						//!< src & dest	(1, 1, 1, 1)
		ENUM_BLEND_FACTOR_DST_COLOR, 				//!< src (destR, destG, destB, destA)
		ENUM_BLEND_FACTOR_ONE_MINUS_DST_COLOR, 		//!< src (1-destR, 1-destG, 1-destB, 1-destA)
		ENUM_BLEND_FACTOR_SRC_COLOR,				//!< dest (srcR, srcG, srcB, srcA)
		ENUM_BLEND_FACTOR_ONE_MINUS_SRC_COLOR, 		//!< dest (1-srcR, 1-srcG, 1-srcB, 1-srcA)
		ENUM_BLEND_FACTOR_SRC_ALPHA,				//!< src & dest	(srcA, srcA, srcA, srcA)
		ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		//!< src & dest	(1-srcA, 1-srcA, 1-srcA, 1-srcA)
		ENUM_BLEND_FACTOR_DST_ALPHA,				//!< src & dest	(destA, destA, destA, destA)
		ENUM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		//!< src & dest	(1-destA, 1-destA, 1-destA, 1-destA)
		ENUM_BLEND_FACTOR_SRC_ALPHA_SATURATE,		//!< src (min(srcA, 1-destA), idem, ...)
		ENUM_BLEND_FACTOR_COUNT

		*/



	//测试粒子

	/*p_SpeEffectSet=new CSpeEffectSet();
*/
	/*p_ParticleSystem=new CParticleSystem();

	p_ParticleSystem->setDriver(videoDriver);
	//CParticleEmiter * emiter=p_ParticleSystem->CreateCircleEmiter();
	//p_ParticleSystem->setEmiter(emiter);
    IReadStream * p_FileReader=engine->getFileSystem()->createAndOpenReadFileStream("exportParticle/5.par");
	p_ParticleSystem->load(p_FileReader);
	p_FileReader->drop();
	//delete p_FileReader;
	p_ParticleSystem->setPos(position2df(0,0));

   
*/

	/************************************************************************/
	/* 测试用数据                                                           */
	/************************************************************************/
   /* p_ParticleSystem->setTexture("fire.png");
	p_ParticleSystem->setEmiteLife(400);
	p_ParticleSystem->setEmiteAlways(false);
	p_ParticleSystem->setPosVar(position2df(0,0));
	p_ParticleSystem->setSpeed(165.0f);
	p_ParticleSystem->setSpeedVar(45);
	p_ParticleSystem->setAngle(180);
	p_ParticleSystem->setAngleVar(0);
	p_ParticleSystem->setEmissionRate(450);
	p_ParticleSystem->setEmissionRateVar(0);
	p_ParticleSystem->setMaxParticleCount(0);
	p_ParticleSystem->setTimeToLive(450);
	p_ParticleSystem->setTimeToLiveVar(250);
	p_ParticleSystem->setStartDimSize(dimension2df(64,34));
	p_ParticleSystem->setStartDimSizeVar(dimension2df(16,16));
	p_ParticleSystem->setEndDimSize(position2df(0,39));
	p_ParticleSystem->setEndDimSizeVar(position2df(0,0));
	p_ParticleSystem->setMixType(ENUM_BLEND_FACTOR_SRC_ALPHA,ENUM_BLEND_FACTOR_ONE);
    p_ParticleSystem->setAlphaSource(0);
	p_ParticleSystem->setModufunc(ENUM_MODULATE_1X);
	p_ParticleSystem->setStartEqualEnd(false);
	p_ParticleSystem->setShapeOfSquare(false);
	p_ParticleSystem->setStartColor(SColorf(0.5,1,1,0.1));
	p_ParticleSystem->setStartColorVar(SColorf(0.25,0.39,0.42,0.3));
	p_ParticleSystem->setEndColor(SColorf(0.5,0.5,1,0.73));
	p_ParticleSystem->setEndColorVar(SColorf(0.19,0.34,0.46,0.12));
	p_ParticleSystem->setStartColorEqualEnd(false);
	p_ParticleSystem->setStartRotation(0.0f);
	p_ParticleSystem->setStartRotationVar(0);
	p_ParticleSystem->setEndRotation(0);
	p_ParticleSystem->setEndRotationVar(0);
	p_ParticleSystem->setStartRotationEqualEnd(false);
	p_ParticleSystem->setRotationMode((ROTATION_MODE)2);
	p_ParticleSystem->setGravity(position2df(0,1250));
	p_ParticleSystem->setStartRadius(0);
	p_ParticleSystem->setStartRadiusVar(45);
	p_ParticleSystem->setEndRadius(64);
	p_ParticleSystem->setEndRadiusVar(32);
	p_ParticleSystem->setRotatePerSecond(0);
	p_ParticleSystem->setRotatePerSecondVar(360);
	p_ParticleSystem->setStartCircleAngle(0);
	p_ParticleSystem->setStartCircleAngleVar(360);
	p_ParticleSystem->setStartRadiusEqualEnd(true);


	CSpeEffect* p_SpeEffect=new CSpeEffect();

	p_SpeEffect->speType=PARTICLE;
	p_SpeEffect->m_SpeEffect.p_ParticleSystem=p_ParticleSystem;
	p_SpeEffect->setStartTime(0);
	p_SpeEffect->setEndTime(5000);

	p_SpeEffectSet->SpeArray.push_back(p_SpeEffect);

*/	
	CParticleEmiter::setRandomizer(randomizer);

  p_SpeEffectSet=new CSpeEffectSet();
   p_SpeEffectSet->setVideoDriver(videoDriver);
   p_SpeEffectSet->setGraphicsAdapter(gfAdapter);
   IReadStream *p_FileReader=engine->getFileSystem()->createAndOpenReadFileStream("exportSpe/5.EFF");
   p_SpeEffectSet->load(p_FileReader);
   p_FileReader->drop();
   
   p_SpeEffectSet->setPos(position2di(0,0));

   IMaterial* material;
   IShap *shap;
   IUnit* unit;
   IEntity* entity;

   shap=geometryFty->createCube(50,50,50);
   unit=geometryFty->createUnit(shap);
   entity=geometryFty->createEntity(unit);
   cubeModel=sceneMgr->addModel(entity);
   material=cubeModel->getMaterial(0);
   material->setMaterialType(ENUM_MATERIAL_TYPE_SOLID);
  // material->setMaterialType(ENUM_MATERIAL_TYPE_TRANSPARENT);
   //material->setFilterMode(0,ENUM_FILTER_MODE_NEAREST);
   cubeModel->setPosition(core::vector3df(100,100,0));
   //material->setTexture(0,videoDriver->getTexture("png8/120.png"));
   material->setTexture(0,videoDriver->getTexture("test.png"));
   shap->drop();
   unit->drop();
   entity->drop();
 
	return true;


}

void resize(u32 width,u32 height)
{

	engine->onResize(width,height);


}


void drawFrame(){


	videoDriver->begin();

	const core::vector3df crot=cubeModel->getRotation();
	cubeModel->setRotation(core::vector3df(crot.x,crot.y+0.5f ,crot.z));

	sceneMgr->render(videoDriver);
  
   // p_BillboardAmt->update(100);

	//p_BillboardAmt->draw();
   

	gfAdapter->clearZ(-1000);

	core::rectf r(0,0,1,1);
	//for(u32 i=0;i<100;++i){
	gfAdapter->drawRegion("shadow.png",r,250,120,128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER),true,0xFF0000FF);
	gfAdapter->drawRegion("trans.png",r,0,120,128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_LEFT|MASK_ACTHOR_TOP),false,0xFF0000FF);
	gfAdapter->drawRegion("trans.png",r,100,120,128,64,ENUM_TRANS_ROT180);
	gfAdapter->drawRegion("trans.png",r,200,120,128,64,ENUM_TRANS_MIRROR);
	gfAdapter->drawRegion("trans.png",r,300,120,128,64,ENUM_TRANS_MIRROR_ROT180);
	

	if(lastTime!=0)
	{
		
	   int now=timer->getRealTime();

      // p_ParticleSystem->update((now-lastTime));

	//   p_BillboardAmt->update(now-lastTime);
	  
	  p_SpeEffectSet->update(now-lastTime);

	   lastTime=now;
	
	 //  p_ParticleSystem->draw();

	   //p_BillboardAmt->draw();

	    p_SpeEffectSet->draw();


	}
	else
		lastTime=timer->getRealTime();

	gfAdapter->drawRegion("test.png",r,200,120,128,64,ENUM_TRANS_ROT90);
	gfAdapter->drawRegion("shadow.png",r,50,170,128,64,ENUM_TRANS_NONE,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER),true);
	gfAdapter->drawRegion("trans.png",r,100,30,128,64,ENUM_TRANS_MIRROR_ROT90);
	gfAdapter->drawRegion("trans.png",r,200,320,128,64,ENUM_TRANS_MIRROR_ROT270,(MASK_ACTHOR)(MASK_ACTHOR_HCENTER|MASK_ACTHOR_VCENTER));
	gfAdapter->drawRegion("trans.png",r,300,320,128,64,ENUM_TRANS_ROT270,(MASK_ACTHOR)(MASK_ACTHOR_RIGHT|MASK_ACTHOR_BOTTOM));

	gfAdapter->drawImage("shadow.png",0,0,64,32,50,50,true,0xAA000000);


	gfAdapter->render();

	// p_ParticleSystem->update(25);
	// p_ParticleSystem->draw();

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();



}
void destroy(){

	engine->drop();
	delete params.pEventReceiver;
	//delete p_BillboardAmt;
	//p_BillboardAmt=NULL;
	//delete p_ParticleSystem;
   // p_ParticleSystem=NULL;
	delete p_SpeEffectSet;
	p_SpeEffectSet=NULL;

}

IYonEngine* getEngine(){
	return engine;
}