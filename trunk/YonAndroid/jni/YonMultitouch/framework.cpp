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

IModel* teapotModel=NULL;

class MyEventReceiver : public IEventReceiver{
	enum ENUM_MODE{
		ENUM_MODE_NONE = 0,
		ENUM_MODE_DRAG,
		ENUM_MODE_SCALE
	};
	static c8* MODE_STRING[];

	struct EventEntry{
		s32 id;
		core::vector2di start,end;
	};

	class AccessClass{
	public:
		AccessClass(const ENUM_MODE& m):mode(m)
		{
		}
		void operator = (const ENUM_MODE& m)
		{
			logger->debug("%s->%s\n",MODE_STRING[mode],MODE_STRING[m]);
			mode=m;
		}

		operator ENUM_MODE()
		{
			return mode;
		}

	ENUM_MODE mode;
	};

	AccessClass mode;

	//core::vector2di start1,start2,end1,end2;
	EventEntry input1,input2;
	core::matrix4f mat;
public:
	MyEventReceiver():mode(ENUM_MODE_NONE),mat(core::IDENTITY_MATRIX){}

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
				//logger->debug("[LP]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				mode=ENUM_MODE_DRAG;
				//start1.set(evt.mouseInput.x,evt.mouseInput.y);
				input1.start.set(evt.mouseInput.x,evt.mouseInput.y);
				result=true;
				break;
			case event::ENUM_MOUSE_INPUT_TYPE_LUP:
				//logger->debug("[LR]%d,%d\n",evt.mouseInput.x,evt.mouseInput.y);
				mode=ENUM_MODE_NONE;
				mat.makeIdentity();
				changed=true;
				result=true;
				break;
			case event::ENUM_MOUSE_INPUT_TYPE_MOVE:
				if(mode==ENUM_MODE_DRAG)
				{
					move=true;
					//end1.set(evt.mouseInput.x,evt.mouseInput.y);
					input1.end.set(evt.mouseInput.x,evt.mouseInput.y);
				}
				result=true;
				break;
			}
			break;
		case event::ENUM_EVENT_TYPE_TOUCH:
			switch(evt.touchInput.type)
			{
			case event::ENUM_TOUCH_INPUT_TYPE_DOWN:
				//logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				if(evt.touchInput.ids[0]==1)
				{
					mode=ENUM_MODE_SCALE;
					//start2.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
					//end2=start2;
					//logger->debug("end2=start2:%d,%d\n",start2.x,start2.y);
					changed=true;
				}
				else if(evt.touchInput.ids[0]==0)
				{
					mode=ENUM_MODE_DRAG;
					//start1.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
					//end1=start1;
					//logger->debug("end1=start1:%d,%d\n",start1.x,start1.y);
					changed=true;
				}
				result=true;
				break;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				//logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				if(evt.touchInput.ids[0]==1)
				{
					mode=ENUM_MODE_DRAG;
					changed=true;
				}
				else if(evt.touchInput.ids[0]==0)
				{
					mode=ENUM_MODE_NONE;
					mat.makeIdentity();
					changed=true;
				}
				result=true;
				break;
			case event::ENUM_TOUCH_INPUT_TYPE_MOVE:
				move=true;
				if(mode==ENUM_MODE_DRAG)
				{
					//end1.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
					//logger->debug("end1:%d,%d\n",end1.x,end1.y);
				}
				else if(mode==ENUM_MODE_SCALE)
				{
					//end1.set(evt.touchInput.xs[0],evt.touchInput.ys[0]);
					//logger->debug("end1:%d,%d\n",end1.x,end1.y);
					//end2.set(evt.touchInput.xs[1],evt.touchInput.ys[1]);
					//logger->debug("end2:%d,%d\n",end2.x,end2.y);
				}
				result=true;
				break;
			}
			break;
		}
		if(move)
		{
			switch(mode)
			{
			case ENUM_MODE_DRAG:
				mat.setTranslation(input1.end.x-input1.start.x,input1.start.y-input1.end.y,0);
				changed=true;
				break;
			case ENUM_MODE_SCALE:
				/*f32 scale=start1.getDistanceFrom(start2);
				if(core::equals(scale,0.0f))
					scale=1;
				else
					scale=end2.getDistanceFrom(end1)/scale;
				logger->debug("scale:%.2f\n",scale);
				mat.setScale(scale,scale,scale);*/
				changed=true;
				break;
			}
			
		}
		if(changed)
		{
			mat.print();
			teapotModel->setTransformation(mat);
		}
		
		return result;
	}
};
c8* MyEventReceiver::MODE_STRING[]={
	"NONE",
	"DRAG",
	"SCALE"
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

	IMaterial* material;
	IShap *shap;
	IUnit* unit;
	IEntity* entity;

	shap=geometryFty->createTeapot(2,video::COLOR_BLUE);
	unit=geometryFty->createUnit(shap);
	entity=geometryFty->createEntity(unit);
	teapotModel=sceneMgr->addModel(entity);
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