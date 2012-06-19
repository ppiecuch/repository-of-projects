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

IModel* teapotModel1=NULL;
IModel* teapotModel2=NULL;
core::position3df teapotPos1;
core::position3df teapotPos2;

class MyEventReceiver : public IEventReceiver{

	struct EventEntry{

		EventEntry():id(-1),start(core::ORIGIN_POSITION2DI),end(core::ORIGIN_POSITION2DI){}

		s32 id;
		core::vector2di start,end;

		void operator = (const EventEntry& other)
		{
			id=other.id;
			start=other.start;
			end=other.end;
		}
	};

	EventEntry inputLeft,inputRight;

	//×óÏÂ½Ç²ÅÎª×ó
	bool inLeft(s32 x,s32 y)
	{
		return x<200&&y>200;
	}
public:
	MyEventReceiver(){}

	virtual bool onEvent(const SEvent& evt){
		bool move=false;
		bool result=false;
		bool changed=false;
		//Logger->debug("evt.type:%d\n",evt.type);
		switch(evt.type)
		{
		case event::ENUM_EVENT_TYPE_MOUSE:
			switch(evt.mouseInput.type)
			{
			case event::ENUM_MOUSE_INPUT_TYPE_LDOWN:
				if(inLeft(evt.mouseInput.x,evt.mouseInput.y))
				{
					inputLeft.id=0;
					inputLeft.start.set(evt.mouseInput.x,evt.mouseInput.y);
					inputLeft.end=inputLeft.start;
				}
				else
				{
					inputRight.id=0;
					inputRight.start.set(evt.mouseInput.x,evt.mouseInput.y);
					inputRight.end=inputRight.start;
				}
				result=true;
				break;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				if(inputLeft.id==0)
				{
					inputLeft.id=-2;
					inputLeft.end=inputLeft.start=core::ORIGIN_POSITION2DI;
				}
				else if(inputRight.id==0)
				{
					inputRight.id=-2;
					inputRight.end=inputRight.start=core::ORIGIN_POSITION2DI;
				}
				result=true;
				break;
			case event::ENUM_MOUSE_INPUT_TYPE_MOVE:
				if(inputLeft.id==0)
				{
					inputLeft.end.set(evt.mouseInput.x,evt.mouseInput.y);
				}
				else if(inputRight.id==0)
				{
					inputRight.end.set(evt.mouseInput.x,evt.mouseInput.y);			
				};
				result=true;
				break;
			}
			break;
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.touchInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				if(inLeft(evt.touchInput.xs[0],evt.touchInput.ys[0]))
				{
					if(inputLeft.id<0)
					{
						inputLeft.id=evt.touchInput.ids[0];
						inputLeft.start.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
						inputLeft.end=inputLeft.start;
					}
				}
				else
				{
					if(inputRight.id<0)
					{
						inputRight.id=evt.touchInput.ids[0];
						inputRight.start.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
						inputRight.end=inputRight.start;
					}
				}
				result=true;
				break;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				if(inputLeft.id==evt.touchInput.ids[0])
				{
					inputLeft.id=-2;
					inputLeft.end=inputLeft.start=core::ORIGIN_POSITION2DI;
				}
				else if(inputRight.id==evt.touchInput.ids[0])
				{
					inputRight.id=-2;
					inputRight.end=inputRight.start=core::ORIGIN_POSITION2DI;
				}
				break;
			case event::ENUM_TOUCH_INPUT_TYPE_MOVE:
				for(u32 i=0;i<evt.touchInput.count;++i)
				{
					if(evt.touchInput.ids[i]==inputLeft.id)
					{
						inputLeft.end.set(evt.touchInput.xs[i],evt.touchInput.ys[i]);
						logger->debug("move left:%d,%d\n",inputLeft.end.x,inputLeft.end.y);
					}
					else if(evt.touchInput.ids[i]==inputRight.id)
					{
						inputRight.end.set(evt.touchInput.xs[i],evt.touchInput.ys[i]);
						logger->debug("move right:%d,%d\n",inputRight.end.x,inputRight.end.y);
					}
				}
				result=true;
				break;
			}
			break;
		}

		if(inputLeft.id!=-1)
		{
			core::vector2di d=inputLeft.end-inputLeft.start;
			core::position3df n=core::position3df(teapotPos1.x+d.x,teapotPos1.y-d.y,0);
			teapotModel1->setPosition(n);
		}
		if(inputRight.id!=-1)
		{
			core::vector2di d=inputRight.end-inputRight.start;
			core::position3df n=core::position3df(teapotPos2.x+d.x,teapotPos2.y-d.y,0);
			teapotModel2->setPosition(n);
		}
		
		return result;
	}
};

bool init(void *pJNIEnv,u32 width,u32 height){
	params.windowSize.w=400;
	params.windowSize.h=400;
	params.pJNIEnv=pJNIEnv;
	params.fpsLimit=0;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(ENUM_CAMERA_TYPE_ORTHO,core::vector3df(0,0,300)); 
	logger=Logger;
	randomizer=engine->getRandomizer();
	timer=engine->getTimer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel1=sceneMgr->addModel(entity);
	teapotPos1.set(100.0f-videoDriver->getCurrentRenderTargetSize().w/2,-videoDriver->getCurrentRenderTargetSize().h/2+100.0f,0);
	teapotModel1->setPosition(teapotPos1);
	shap->drop();
	unit->drop();
	entity->drop();

	shap=geometryFty->createTeapot(2,video::COLOR_GREEN);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel2=sceneMgr->addModel(entity);
	teapotPos2.set(videoDriver->getCurrentRenderTargetSize().w/2-100.0f,-videoDriver->getCurrentRenderTargetSize().h/2+200.0f,0);
	teapotModel2->setPosition(teapotPos2);
	shap->drop();
	unit->drop();
	entity->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	sceneMgr->render(videoDriver);

	Logger->drawString(videoDriver,core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}