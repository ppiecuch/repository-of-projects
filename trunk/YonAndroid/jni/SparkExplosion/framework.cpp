#include "SPK.h"
#include "SPK_YON.h"
using namespace SPK;
using namespace SPK::YON;
#include "framework.h"

#include <ctime>

SYonEngineParameters params;
IYonEngine* engine=NULL;
IVideoDriver* videoDriver=NULL;
IAudioDriver* audioDriver=NULL;
ISceneManager* sceneMgr=NULL;
IGraphicsAdapter* gfAdapter=NULL;
IFileSystem* fs=NULL;
ICamera* pCamera=NULL;
//IRandomizer* randomizer=NULL;
ITimer* timer=NULL;


float angleY = 0.0f;
float angleX = 45.0f;
float camPosZ = 5.0f;

float step = 0.0f;
// random seed
unsigned int SPK::randomSeed = static_cast<unsigned int>(time(NULL));
System* particleSystem = NULL;

// setup some useful variables
float curtime,oldtime,deltaTime;

std::list<System*> particleSystems;

SPK_ID BaseSystemID = -1;


// Creates a particle system from the base system
System* createParticleSystem(const Vector3D& pos)
{
	// Creates a copy of the base system
	System* system = SPK_Copy(System,BaseSystemID);

	// Locates the system at the given position
	system->setTransformPosition(pos);
	system->updateTransform(); // updates the world transform of system and its children

	return system;
}

// destroy a particle system
void destroyParticleSystem(System*& system)
{
	// Destroys the given system
	SPK_Destroy(system);
	system = NULL;
}


class MyEventReceiver : public IEventReceiver{
public:
	virtual bool onEvent(const SEvent& evt){
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(evt.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_DOZE:
				YON_DEBUG("DOZE\r\n");
				break;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				YON_DEBUG("WAKE\r\n");
				break;
			}
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				YON_DEBUG("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				return true;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				YON_DEBUG("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				Vector3D position(random(-2.0f,2.0f),random(-2.0f,2.0f),random(-2.0f,2.0f));
				particleSystems.push_back(createParticleSystem(position));
				return true;
			}
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//YON_DEBUG("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//YON_DEBUG("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			}
		}
		return false;
	}
};

// Creates the base system and returns its ID
SPK_ID createParticleSystemBase(ITexture* textureExplosion,ITexture* textureFlash,ITexture* textureSpark1,ITexture* textureSpark2,ITexture* textureWave)
{
	///////////////
	// Renderers //
	///////////////

	// smoke renderer
	YONQuadRenderer* smokeRenderer = YONQuadRenderer::create(engine);
	smokeRenderer->setTexturingMode(TEXTURE_2D);
	smokeRenderer->setTexture(textureExplosion);
	//smokeRenderer->setTextureBlending(GL_MODULATE);
	smokeRenderer->setAtlasDimensions(2,2); // uses 4 different patterns in the texture
	smokeRenderer->setBlending(BLENDING_ALPHA);
	smokeRenderer->enableRenderingHint(DEPTH_WRITE,false);
	smokeRenderer->setShared(true);

	
	// flame renderer
	YONQuadRenderer* flameRenderer = YONQuadRenderer::create(engine);
	flameRenderer->setTexturingMode(TEXTURE_2D);
	flameRenderer->setTexture(textureExplosion);
	//flameRenderer->setTextureBlending(GL_MODULATE);
	flameRenderer->setAtlasDimensions(2,2);
	flameRenderer->setBlending(BLENDING_ADD);
	flameRenderer->enableRenderingHint(DEPTH_WRITE,false);
	flameRenderer->setShared(true);

	// flash renderer
	YONQuadRenderer* flashRenderer = YONQuadRenderer::create(engine);
	flashRenderer->setTexturingMode(TEXTURE_2D);
	flashRenderer->setTexture(textureFlash);
	//flashRenderer->setTextureBlending(GL_REPLACE);
	flashRenderer->setBlending(BLENDING_ADD);
	flashRenderer->enableRenderingHint(DEPTH_WRITE,false);
	flashRenderer->setShared(true);

	// spark 1 renderer
	YONQuadRenderer* spark1Renderer = YONQuadRenderer::create(engine);
	spark1Renderer->setTexturingMode(TEXTURE_2D);
	spark1Renderer->setTexture(textureSpark1);
	//spark1Renderer->setTextureBlending(GL_REPLACE);
	spark1Renderer->setBlending(BLENDING_ADD);
	spark1Renderer->enableRenderingHint(DEPTH_WRITE,false);
	spark1Renderer->setOrientation(DIRECTION_ALIGNED); // sparks are oriented function o their velocity
	spark1Renderer->setScale(0.05f,1.0f); // thin rectangles
	spark1Renderer->setShared(true);

	// spark 2 renderer
	YONRenderer* spark2Renderer = NULL;
#if 0
	if (YONPointRenderer::loadGLExtPointSprite() && YONPointRenderer::loadGLExtPointParameter()) // uses point sprite if possible
	{
		//YONPointRenderer::setPixelPerUnit(45.0f * PI / 180.f,screenHeight);
		YONPointRenderer* pointRenderer = YONPointRenderer::create();
		pointRenderer->setType(POINT_SPRITE);
		pointRenderer->setTexture(textureSpark2);
		//pointRenderer->setTextureBlending(GL_MODULATE);
		//pointRenderer->enableWorldSize(true);
		pointRenderer->setSize(0.02f);
		spark2Renderer = pointRenderer;
	}
	else
#endif
	{
		YONQuadRenderer* quadRenderer = YONQuadRenderer::create(engine);
		quadRenderer->setTexturingMode(TEXTURE_2D);
		quadRenderer->setTexture(textureSpark2);
		//quadRenderer->setTextureBlending(GL_MODULATE);
		quadRenderer->setScale(0.02f,0.02f);
		spark2Renderer = quadRenderer;
	}
	spark2Renderer->setBlending(BLENDING_ADD);
	spark2Renderer->enableRenderingHint(DEPTH_WRITE,false);
	spark2Renderer->setShared(true);

	// wave renderer
	YONQuadRenderer* waveRenderer = YONQuadRenderer::create(engine);
	waveRenderer->setTexturingMode(TEXTURE_2D);
	waveRenderer->setTexture(textureWave);
	//waveRenderer->setTextureBlending(GL_MODULATE);
	waveRenderer->setBlending(BLENDING_ALPHA);
	waveRenderer->enableRenderingHint(DEPTH_WRITE,false);
	waveRenderer->enableRenderingHint(ALPHA_TEST,true); // uses the alpha test
	waveRenderer->setAlphaTestThreshold(0.0f);
	waveRenderer->setOrientation(FIXED_ORIENTATION); // the orientatin is fixed
	waveRenderer->lookVector.set(0.0f,1.0f,0.0f);
	waveRenderer->upVector.set(1.0f,0.0f,0.0f); // we dont really care about the up axis
	waveRenderer->setShared(true);

	////////////
	// Models //
	////////////

	Interpolator* interpolator = NULL; // pointer to an interpolator that is used to retrieve interpolators	

	// smoke model
	Model* smokeModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_SIZE | FLAG_ANGLE,
		FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_ALPHA);
	smokeModel->setParam(PARAM_RED,0.2f);
	smokeModel->setParam(PARAM_GREEN,0.2f);
	smokeModel->setParam(PARAM_BLUE,0.2f);
	smokeModel->setParam(PARAM_SIZE,0.6f,0.8f,1.0f,1.4f);
	smokeModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
	smokeModel->setParam(PARAM_ANGLE,0.0f,PI * 0.5f,0.0f,PI * 0.5f);
	smokeModel->setLifeTime(2.5f,3.0f);
	smokeModel->setShared(true);

	interpolator = smokeModel->getInterpolator(PARAM_ALPHA);
	interpolator->addEntry(0.0f,0.0f);
	interpolator->addEntry(0.4f,0.4f,0.6f);
	interpolator->addEntry(0.6f,0.4f,0.6f);
	interpolator->addEntry(1.0f,0.0f);

	
	// flame model
	Model* flameModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_ANGLE | FLAG_RED | FLAG_GREEN | FLAG_BLUE,
		FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_SIZE | FLAG_ALPHA);
	flameModel->setParam(PARAM_RED,1.0f,0.2f);
	flameModel->setParam(PARAM_GREEN,0.5f,0.2f);
	flameModel->setParam(PARAM_BLUE,0.2f,0.2f);
	flameModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
	flameModel->setParam(PARAM_ANGLE,0.0f,PI * 0.5f,0.0f,PI * 0.5f);
	flameModel->setLifeTime(1.5f,2.0f);
	flameModel->setShared(true);

	interpolator = flameModel->getInterpolator(PARAM_SIZE);
	interpolator->addEntry(0.0f,0.25f);
	interpolator->addEntry(0.02f,0.6f,0.8f);
	interpolator->addEntry(1.0f,1.0f,1.4f);

	interpolator = flameModel->getInterpolator(PARAM_ALPHA);
	interpolator->addEntry(0.5f,1.0f);
	interpolator->addEntry(1.0f,0.0f);

	// flash model
	Model* flashModel = Model::create(FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE,
		FLAG_NONE,
		FLAG_ANGLE,
		FLAG_ALPHA | FLAG_SIZE);
	flashModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI);
	flashModel->setLifeTime(0.5f,0.5f);
	flashModel->setShared(true);

	interpolator = flashModel->getInterpolator(PARAM_SIZE);
	interpolator->addEntry(0.0f,0.25f);
	interpolator->addEntry(0.1f,1.0f,2.0f);

	interpolator = flashModel->getInterpolator(PARAM_ALPHA);
	interpolator->addEntry(0.0f,1.0f);
	interpolator->addEntry(0.4f,0.0f);

	// spark 1 model
	Model* spark1Model = Model::create(FLAG_SIZE | FLAG_ALPHA,
		FLAG_ALPHA,
		FLAG_SIZE);
	spark1Model->setParam(PARAM_ALPHA,1.0f,0.0f);
	spark1Model->setParam(PARAM_SIZE,0.2f,0.4f);
	spark1Model->setLifeTime(0.2f,1.0f);
	spark1Model->setShared(true);

	// spark 2 model
	Model* spark2Model = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA,
		FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA,
		FLAG_GREEN);
	spark2Model->setParam(PARAM_ALPHA,1.0f,0.0f);
	spark2Model->setParam(PARAM_RED,1.0f);
	spark2Model->setParam(PARAM_GREEN,1.0f,1.0f,0.3f,1.0f);
	spark2Model->setParam(PARAM_BLUE,0.7f,0.3f);
	spark2Model->setLifeTime(1.0f,3.0f);
	spark2Model->setShared(true);

	// wave model
	Model* waveModel = Model::create(FLAG_ALPHA | FLAG_SIZE,
		FLAG_SIZE | FLAG_ALPHA);
	waveModel->setParam(PARAM_SIZE,0.0f,4.0f);
	waveModel->setParam(PARAM_ALPHA,0.2f,0.0f);
	waveModel->setLifeTime(0.8f,0.8f);
	waveModel->setShared(true);

	//////////////
	// Emitters //
	//////////////

	// This zone will be used by several emitters
	Sphere* explosionSphere = Sphere::create(Vector3D(0.0f,0.0f,0.0f),0.4f);

	// smoke emitter
	RandomEmitter* smokeEmitter = RandomEmitter::create();
	smokeEmitter->setZone(Sphere::create(Vector3D(0.0f,0.0f,0.0f),0.6f),false);
	smokeEmitter->setFlow(-1);
	smokeEmitter->setTank(15);
	smokeEmitter->setForce(0.02f,0.04f);


	// flame emitter
	NormalEmitter* flameEmitter = NormalEmitter::create();
	flameEmitter->setZone(explosionSphere);
	flameEmitter->setFlow(-1);
	flameEmitter->setTank(15);
	flameEmitter->setForce(0.06f,0.1f);

	// flash emitter
	StaticEmitter* flashEmitter = StaticEmitter::create();
	flashEmitter->setZone(Sphere::create(Vector3D(0.0f,0.0f,0.0f),0.1f));
	flashEmitter->setFlow(-1);
	flashEmitter->setTank(3);

	// spark 1 emitter
	NormalEmitter* spark1Emitter = NormalEmitter::create();
	spark1Emitter->setZone(explosionSphere);
	spark1Emitter->setFlow(-1);
	spark1Emitter->setTank(20);
	spark1Emitter->setForce(2.0f,3.0f);

	// spark 2 emitter
	NormalEmitter* spark2Emitter = NormalEmitter::create();
	spark2Emitter->setZone(explosionSphere);
	spark2Emitter->setFlow(-1);
	spark2Emitter->setTank(400);
	spark2Emitter->setForce(0.4f,0.8f);

	// wave emitter
	StaticEmitter* waveEmitter = StaticEmitter::create();
	waveEmitter->setZone(Point::create());
	waveEmitter->setFlow(-1);
	waveEmitter->setTank(1);

	////////////
	// Groups //
	////////////

	// smoke group
	Group* smokeGroup = Group::create(smokeModel,15);
	smokeGroup->addEmitter(smokeEmitter);
	smokeGroup->setRenderer(smokeRenderer);
	smokeGroup->setGravity(Vector3D(0.0f,0.05f,0.0f));

	// flame group
	Group* flameGroup = Group::create(flameModel,15);
	flameGroup->addEmitter(flameEmitter);
	flameGroup->setRenderer(flameRenderer);

	// flash group
	Group* flashGroup = Group::create(flashModel,3);
	flashGroup->addEmitter(flashEmitter);
	flashGroup->setRenderer(flashRenderer);

	// spark 1 group
	Group* spark1Group = Group::create(spark1Model,20);
	spark1Group->addEmitter(spark1Emitter);
	spark1Group->setRenderer(spark1Renderer);
	spark1Group->setGravity(Vector3D(0.0f,-1.5f,0.0f));

	// spark 2 group
	Group* spark2Group = Group::create(spark2Model,400);
	spark2Group->addEmitter(spark2Emitter);
	spark2Group->setRenderer(spark2Renderer);
	spark2Group->setGravity(Vector3D(0.0f,-0.3f,0.0f));
	spark2Group->setFriction(0.4f);

	// wave group
	Group* waveGroup = Group::create(waveModel,1);
	waveGroup->addEmitter(waveEmitter);
	waveGroup->setRenderer(waveRenderer);

	////////////
	// System //
	////////////

	System* system = System::create();
	system->addGroup(waveGroup);
	system->addGroup(smokeGroup);
	system->addGroup(flameGroup);
	system->addGroup(flashGroup);
	system->addGroup(spark1Group);
	system->addGroup(spark2Group);

	// Gets a pointer to the base
	return system->getSPKID();
}

bool init(void *pJNIEnv,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.resourcesPath=resPath;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	Logger->setAppender(MASK_APPENDER_VS|MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE|MASK_APPENDER_SCREEN);
	//Logger->setFormat(MASK_FORMAT_LOG);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCameraFPS(NULL,0.01f);
	//randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/spark");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/spark");
#endif

	

	// Sets the update step
	System::setClampStep(true,0.1f);			// clamp the step to 100 ms
	System::useAdaptiveStep(0.001f,0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)

	// Loads particle texture
	ITexture* textureExplosion=videoDriver->getTexture("explosion.png");

	ITexture* textureFlash=videoDriver->getTexture("flash.png");

	ITexture* textureSpark1=videoDriver->getTexture("spark1.png");

	ITexture* textureSpark2=videoDriver->getTexture("point.png");

	ITexture* textureWave=videoDriver->getTexture("wave.png");

	// creates the base system
	BaseSystemID = createParticleSystemBase(textureExplosion,textureFlash,textureSpark1,textureSpark2,textureWave);

	// setup some useful variables
	curtime=(f32)timer->getTime() / 1000.0f;

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){
	videoDriver->begin(true,true,COLOR_DEFAULT);

	oldtime = curtime;
	curtime = (f32)timer->getTime() / 1000.0f;
	deltaTime = curtime - oldtime;

	std::list<System*>::iterator it = particleSystems.begin();
	while(it != particleSystems.end())
	{
		// Updates the particle systems
		if (!(*it)->update(deltaTime))
		{
			// If a system is sleeping, destroys it
			destroyParticleSystem(*it);
			// And erases its entry in the container
			it = particleSystems.erase(it);
		}
		else
		{
			(*it)->render();
			++it;
		}
		
	}

	sceneMgr->render(videoDriver);
	
	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,PTL:%d,DWL:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),particleSystem?particleSystem->getNbParticles():-1,videoDriver->getDrawCall()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->setMaterial(video::DEFAULT_MATERIAL);
	videoDriver->setTransform(ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
	videoDriver->draw3DLine(core::vector3df(100,0,0),core::IDENTITY_VECTOR3DF,video::COLOR_RED);
	videoDriver->draw3DLine(core::vector3df(0,100,0),core::IDENTITY_VECTOR3DF,video::COLOR_GREEN);
	videoDriver->draw3DLine(core::vector3df(0,0,100),core::IDENTITY_VECTOR3DF,video::COLOR_BLUE);

	videoDriver->end();
}
void destroy(){
	SPKFactory::getInstance().destroyAll();
	SPKFactory::destroyInstance();
	engine->drop();
	delete params.pEventReceiver;
	engine=NULL;
}

IYonEngine* getEngine(){
	return engine;
}