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
ITimer* timer=NULL;

ISceneNode* cubeModel=NULL;
ISceneNode* planeModel=NULL;
f32 factor=1.1f;

#include "SDynamicShap.h"
#include "DarkManager.h"
#include "SpriteLoader.h"
#include "AnimationLoader.h"

#define INTERVAL 93

u16 Sprite::Indices[6]={0,1,2,0,2,3};

scene::SDynamicShap3D shap;



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

void render(AnimationGroup* group,u32 animIndex,u32 cellIndex,f32 x,f32 y);

class Actor{
	AnimationGroup* m_pGroup;
	s32 m_iAnimIndex;
	s32 m_iCellIndex;
	u32 m_uCellCount;
	u32 m_uStartTime;
public:
	Actor(AnimationGroup* group):m_pGroup(group),m_iAnimIndex(0),m_iCellIndex(0),m_uStartTime(0){}

	void play(const c8* name);
	void onAnimate(u32 time);
	void render();
};

void Actor::play(const c8* name)
{
	for(u32 i=0;i<m_pGroup->Animiations.size();++i)
	{
		if(m_pGroup->Animiations[i].Name==name)
		{
			m_iAnimIndex=i;
			m_iCellIndex=0;
			m_uCellCount=m_pGroup->Animiations[i].Cells.size();
			m_uStartTime=0;
		}
	}
}

void Actor::onAnimate(u32 time)
{
	if(m_uStartTime==0)
		m_uStartTime=time;

	u32 diff=(time-m_uStartTime)/INTERVAL;
	m_iCellIndex=diff%m_uCellCount;
}

void Actor::render()
{
	::render(m_pGroup,m_iAnimIndex,m_iCellIndex,50,50);
}

DarkManager* darkMgr=NULL;
AnimationGroup* group=NULL;
Actor* actor=NULL;

void render(AnimationGroup* group,u32 animIndex,u32 cellIndex,f32 x,f32 y)
{
	if(group->Sheet==NULL)
		group->Sheet=darkMgr->getSpritesheet(group->SpriteSheet);

	video::ITexture* tex=group->Sheet->Texture;
	static SMaterial material(video::ENUM_MATERIAL_TYPE_TRANSPARENT);
	material.setTexture(0,tex);
	material.CullingMode=video::ENUM_CULLING_MODE_NONE;

	videoDriver->setMaterial(material);

	Animation::Cell& cell=group->Animiations[animIndex].Cells[cellIndex];
	core::array<scene::SVertex>& vertices=shap.getVertexArray();
	core::array<u16>& indices=shap.getIndexArray();
	vertices.set_used(0);
	indices.set_used(0);
	for(u32 i=0;i<cell.Sprites.size();++i)
	{
		Animation::SpriteExt& ext=cell.Sprites[i];
		if(cell.Sprites[i].Sprite==NULL)
		{
			for(u32 i=0;i<group->Sheet->Sprites.size();++i)
			{
				if(group->Sheet->Sprites[i].FullName==ext.Name)
				{
					ext.Sprite=&group->Sheet->Sprites[i];
					break;
				}
			}
		}

		ext.Sprite->setCenterPosition(x+ext.X,y+ext.Y,0);
		shap.append(ext.Sprite->Vertices,4,Sprite::Indices,6);
	}

	videoDriver->drawShap(&shap);
}


bool init(void *pJNIEnv,const c8* appPath,const c8* resPath,u32 width,u32 height){
	params.windowSize.w=width;
	params.windowSize.h=height;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.resourcesPath=resPath;
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
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->addWorkingDirectory("../media/");
	fs->addWorkingDirectory("../media/darkFunction");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->addWorkingDirectory("media/");
#endif

	darkMgr=new DarkManager;

	SpriteLoader::setFileSystem(fs);
	SpriteLoader::setVideoDriver(videoDriver);

	AnimationLoader::setFileSystem(fs);

	Spritesheet* de=darkMgr->getSpritesheet("swk.sprites");
	group=darkMgr->getAnimationGroup("swk.anim");

	actor=new Actor(group);
	actor->play("gather");


	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}

void drawFrame(){

	videoDriver->begin(true,true,video::COLOR_DEFAULT);

	sceneMgr->render(videoDriver);

	actor->onAnimate(timer->getRealTime());
	actor->render();
	

	Logger->drawString(videoDriver,core::stringc("FPS:%d,TRI:%d,DCL:%d",videoDriver->getFPS(),videoDriver->getPrimitiveCountDrawn(),videoDriver->getDrawCall()),core::position2di(0,10),COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	delete actor;
	delete darkMgr;
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}