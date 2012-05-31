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

IModel* cubeModel=NULL;
IModel* weedModel=NULL;
IModel* planeModel=NULL;
IModel* teapotModel=NULL;
f32 factor=1.1f;

class CustomModel : public scene::IModel
{
private:
	scene::IUnit* m_pUnit;
public:
	//必要时可以将需要的对象传进来
	CustomModel(IModel* parent,const IGeometryFactory* geometryFty,video::IVideoDriver* driver,
		const core::vector3df& pos=core::vector3df(0,0,0),
		const core::vector3df& rot=core::vector3df(0,0,0),
		const core::vector3df& scale=core::vector3df(1,1,1))
		:IModel(parent,pos,rot,scale),m_pUnit(NULL){

#if 1
		//创建形态[1]：创建一个四边形
		/*
		//  ----------x1,y1
		// |			 |
		// |			 |
		// x0,y0------
		// u3,v3------u2,v2
		// |          |
		// u0,v0------u1,v1
		IShap* createXYRectangle3D(s32 x0,s32 y0,s32 x1,s32 y1,f32 z,f32 u0,f32 v0,f32 u1,f32 v1,f32 u2,f32 v2,f32 u3,f32 v3,const video::SColor& color=video::COLOR_WHITE)
		*/
		IShap* shap=geometryFty->createXYRectangle3D(0,0,120,120,0,0,0,1,0,1,1,0,1);
		m_pUnit=geometryFty->createUnit(shap);
		shap->drop();
#else

		//创建形态[2]：自定义顶点序列
		IShap* shap=geometryFty->createShap(ENUM_VERTEX_TYPE_3V1T1C,0,0);
		core::array<SVertex> vertices;
		vertices.push_back(SVertex(0,0,0,0,0,COLOR_WHITE));
		vertices.push_back(SVertex(120,0,0,0,1,COLOR_WHITE));
		vertices.push_back(SVertex(120,120,0,1,1,COLOR_WHITE));
		vertices.push_back(SVertex(0,120,0,1,0,COLOR_WHITE));
		core::array<u16> indices;
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};
		indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			indices.push_back(u[i]);
		shap->append(vertices.const_pointer(),vertices.size(),indices.const_pointer(),indices.size());
		m_pUnit=geometryFty->createUnit(shap);
		shap->drop();
#endif
		//设置材质
		IMaterial* material=m_pUnit->getMaterial();
		material->setTexture(0,driver->getTexture("aura.png"));
		//material->setTexture(0,driver->getTexture("shanti2.png"));
		material->setMaterialType(ENUM_MATERIAL_TYPE_BLEND);
		material->setBlendSrcFactor(ENUM_BLEND_FACTOR_SRC_ALPHA);
		material->setBlendDstFactor(ENUM_BLEND_FACTOR_ONE);
		//material->setAlphaSource(ENUM_ALPHA_SOURCE_TEXTURE);
	}
	~CustomModel(){
		if(m_pUnit)
			m_pUnit->drop();
	}

	virtual u32 getMaterialCount() const{
		if(m_pUnit==NULL)
			return 0;
		//一个Unit对应一个Material
		return 1;
	}

	virtual video::IMaterial* getMaterial(u32 num) const{
		if(m_pUnit==NULL)
			return NULL;
		return m_pUnit->getMaterial();
	}

	virtual void render(video::IVideoDriver* driver){
		if(m_bVisible==false)
			return;

		driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());
		driver->setMaterial(m_pUnit->getMaterial());
		driver->drawUnit(m_pUnit);
	}
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
				logger->debug("[P]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
				return true;
			case event::ENUM_TOUCH_INPUT_TYPE_UP:
				logger->debug("[R]%.2f,%.2f\n",evt.touchInput.x,evt.touchInput.y);
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
	//params.fpsLimit=10;
	params.pEventReceiver=new MyEventReceiver();
	engine=CreateEngine(params);
	videoDriver=engine->getVideoDriver();
	audioDriver=engine->getAudioDriver();
	sceneMgr=engine->getSceneManager();
	gfAdapter=engine->getGraphicsAdapter();
	const IGeometryFactory* geometryFty=sceneMgr->getGeometryFactory();
	IAnimatorFactory*  animatorFty=sceneMgr->getAnimatorFactory();
	fs=engine->getFileSystem();
	pCamera=sceneMgr->addCamera(core::vector3df(0,0,300));
	logger=Logger;
	randomizer=engine->getRandomizer();

#ifdef YON_COMPILE_WITH_WIN32
	fs->setWorkingDirectory("../media/");
#elif defined(YON_COMPILE_WITH_ANDROID)
	fs->setWorkingDirectory("media/");
#endif

	CustomModel *myNode =new CustomModel(sceneMgr->getRootModel(), geometryFty, videoDriver);
	myNode->drop();

	return true;
}
void resize(u32 width,u32 height){
	engine->onResize(width,height);
}
void drawFrame(){

	videoDriver->begin(true,true,video::SColor(0xFF132E47));

	sceneMgr->render(videoDriver);

	Logger->drawString(core::stringc("FPS:%d",videoDriver->getFPS()),core::ORIGIN_POSITION2DI,COLOR_GREEN);

	videoDriver->end();
}
void destroy(){
	engine->drop();
	delete params.pEventReceiver;
}

IYonEngine* getEngine(){
	return engine;
}