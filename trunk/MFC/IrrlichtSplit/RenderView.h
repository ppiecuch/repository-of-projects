#pragma once
#include "IrrlichtSplitDoc.h"
class CIrrlichtSplitDoc;
class CRenderView;
class RenderEventReceiver;

#pragma comment(lib,"irrlicht.lib")
#include "irrlicht.h"
#include "EditorCamera.h"
#include "afxwin.h"
#include "thread.h"
using namespace irr;
using namespace io;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

#define toString(x) #x

enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};

struct Context
{
	IrrlichtDevice* device;
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;
	scene::ICameraSceneNode* camera;
	SIrrlichtCreationParameters param;
	scene::ISceneNode* cube;
	scene::ISceneNodeAnimator* anim;
	gui::IGUIEnvironment* env;
	ISceneCollisionManager* collMan;
	core::vector3df intersection;
	core::triangle3df hitTriangle;
	scene::ISceneNode * selectedSceneNode;
	core::line3d<f32> ray;
	ITerrainSceneNode* terrain;
	RenderEventReceiver *receiver;
	vector3df position,rotation;
	int w,h;
	HWND hWnd;
	CIrrlichtSplitDoc* pDoc;
};

class RenderEventReceiver:public IEventReceiver
{
private:
	const static long TIMELEN_LIMIT=500;//limit of drag event verification,ms
	const static int TREMBLE_LIMIT_SQUARE=81;//square of tremble boundary
	unsigned long pressTime,now;
	bool moving,pressing;
	vector2d<s32> start,end;
	Context& context;
public:
	RenderEventReceiver(Context& c):context(c),pressing(false){}
	virtual bool OnEvent(const SEvent& event){
		TRACE("OnEvent(),type:%d",event.EventType);
		switch(event.EventType)
		{
		case EET_MOUSE_INPUT_EVENT:
			TRACE("EET_MOUSE_INPUT_EVENT,%d,%d\r\n",event.MouseInput.X,event.MouseInput.Y);
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				TRACE("EMIE_LMOUSE_PRESSED_DOWN,%d,%d\r\n",event.MouseInput.X,event.MouseInput.Y);
				pressTime=GetTickCount();
				this->pressing=true;
				this->moving=false;
				this->start.X=event.MouseInput.X;
				this->start.Y=event.MouseInput.Y;
				this->end.X=event.MouseInput.X;
				this->end.Y=event.MouseInput.Y;
				//LOGD(LOG_TAG,"pressTime:%u",pressTime.tv_usec);
				//return true;
				return context.camera->OnEvent(event);
			case EMIE_LMOUSE_LEFT_UP:
				TRACE("EMIE_LMOUSE_LEFT_UP,%d,%d\r\n",event.MouseInput.X,event.MouseInput.Y);
				this->pressing=false;
				//context.device->getCursorControl()->setPosition(press);
				//context.device->getCursorControl()->setVisible(true);
				if(this->moving){
					this->moving=false;
				}else{
					this->moving=false;

					position2d<s32> pos;
					pos.X=event.MouseInput.X;
					pos.Y=event.MouseInput.Y;
					//TRACE("%d,%d",pos.X,pos.Y);
					//pos.set(context.device->getCursorControl()->getPosition().X, context.device->getCursorControl()->getPosition().Y );
					//TRACE("%d,%d",pos.X,pos.Y);
					context.ray = context.collMan->getRayFromScreenCoordinates(pos,context.camera);
					TRACE("ray:{%.2f,%.2f,%.2f},{%.2f,%.2f,%.2f}",context.ray.start.X,context.ray.start.Y,context.ray.start.Z,
						context.ray.end.X,context.ray.end.Y,context.ray.end.Z);
					context.selectedSceneNode =context.collMan->getSceneNodeAndCollisionPointFromRay(
								context.ray,
								context.intersection, // This will be the position of the collision
								context.hitTriangle, // This will be the triangle hit in the collision
								IDFlag_IsPickable, // This ensures that only nodes that we have
										// set up to be pickable are considered
								0);
				}
				//return true;
				return context.camera->OnEvent(event);
			case EMIE_MOUSE_MOVED:
				//TRACE("EMIE_MOUSE_MOVED,%d,%d\r\n",event.MouseInput.X,event.MouseInput.Y);
				//if(pressing==false)
				//	return true;
				//context.device->getCursorControl()->setVisible(false);
				/*this->start.X=end.X;
				this->start.Y=end.Y;
				this->end.X=event.MouseInput.X;
				this->end.Y=event.MouseInput.Y;
				vector2d<s32> *result=new vector2d<s32>();
				result->X=end.X-start.X;
				result->Y=end.Y-start.Y;
				//tremble checking
				if(moving==false&&result->getLengthSQ()<TREMBLE_LIMIT_SQUARE)
				{
					now=GetTickCount();
					if((unsigned long)(now-pressTime)<TIMELEN_LIMIT)
					{
						TRACE("delta,%u\r\n",now-pressTime);
						//do not support return false,or the program will collapse
						//return false;
						return true;
					}
				}
				this->moving=true;
				//(*drag)(context,result);
				context.rotation.X+=result->Y;
				context.rotation.Y-=result->X;
				context.camera->setRotation(context.rotation);

				delete result;
				return true;*/
				return context.camera->OnEvent(event);
			}
		default:
			TRACE("default2\r\n");
		}
		return context.camera->OnEvent(event);
	}
};


class RenderThread:public Thread
{
private:
	Context& context;
public:
	RenderThread(Context& c):context(c){}
	void renderInPoint()
	{
		//context.terrain->setMaterialFlag(video::EMF_POINTCLOUD, !context.terrain->getMaterial(0).PointCloud);
		context.terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
		context.terrain->setMaterialFlag(video::EMF_POINTCLOUD, true);
	}
	void renderInLine()
	{
		//context.terrain->setMaterialFlag(video::EMF_WIREFRAME, !context.terrain->getMaterial(0).Wireframe);
		context.terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
		context.terrain->setMaterialFlag(video::EMF_WIREFRAME, true);
	}
	void renderInDetail()
	{
		context.terrain->setMaterialFlag(video::EMF_POINTCLOUD, false);
		context.terrain->setMaterialFlag(video::EMF_WIREFRAME, false);
		context.terrain->setMaterialType(video::EMT_DETAIL_MAP);
	}
	void checkDoc()
	{
		if(context.pDoc->m_bUpdated)
		{
			TRACE("update\r\n");
			switch(context.pDoc->m_nRenderMode)
			{
			case CIrrlichtSplitDoc::POINT:
				renderInPoint();
				break;
			case CIrrlichtSplitDoc::LINE:
				renderInLine();
				break;
			case CIrrlichtSplitDoc::PLANE:
				renderInDetail();
				break;
			}
			context.pDoc->m_bUpdated=false;
		}
	}
	void mouseReleased(int x,int y)
	{
		position2d<s32> pos;
		pos.X=x;
		pos.Y=y;
		context.ray = context.collMan->getRayFromScreenCoordinates(pos,context.camera);
		TRACE("ray:{%.2f,%.2f,%.2f},{%.2f,%.2f,%.2f}",context.ray.start.X,context.ray.start.Y,context.ray.start.Z,
						context.ray.end.X,context.ray.end.Y,context.ray.end.Z);
		context.selectedSceneNode =context.collMan->getSceneNodeAndCollisionPointFromRay(
						context.ray,
						context.intersection, // This will be the position of the collision
						context.hitTriangle, // This will be the triangle hit in the collision
						IDFlag_IsPickable, // This ensures that only nodes that we have
										// set up to be pickable are considered
						0);
	}
	void resize(int w,int h)
	{
		irr::core::dimension2d<unsigned int> size;
		size.Width = w;
		size.Height = h;
		//Event handler for resize events. Only used by the engine internally. 
		//Used to notify the driver that the window was resized. Usually, there is no need to call this method.
		context.driver->OnResize(size);

		//Sets the aspect ratio (default: 4.0f / 3.0f). 
		//context.camera->setAspectRatio((float)w/h);
	}
	void init()
	{
		context.param.WindowId = reinterpret_cast<void*>(context.hWnd);
		//EDT_OPENGL用了显示不正常,原因待查
		context.param.DriverType = irr::video::EDT_OPENGL;
		context.device = irr::createDeviceEx(context.param);
		//device->setResizeAble(true);
		context.env = context.device->getGUIEnvironment();
		context.driver=context.device->getVideoDriver();
		context.smgr=context.device->getSceneManager();

		//create a camera and set its target

		//context.camera = context.smgr->addCameraSceneNode();
		//context.camera->setTarget(core::vector3df(0,0,0));

		//context.camera = context.smgr->addCameraSceneNodeFPS(0,150.0f,1.2f);
		context.camera = new EditorCamera(context.device,context.smgr->getRootSceneNode(),context.smgr,-1,-100.0f,50.0f,50.0f); 
		context.camera->bindTargetAndRotation(true);
		context.position.X=2700*2;
		context.position.Y=655*2;
		context.position.Z=2600*2;
		context.camera->setPosition(context.position);
		context.camera->setTarget(core::vector3df(2397*2,643*2,2700*2));
		//context.camera->setPosition(core::vector3df(50,50,-60));
		//context.camera->setTarget(core::vector3df(-70,30,-60));
		context.camera->setFarValue(42000.0f);
		//context.receiver = new RenderEventReceiver(context);
		//context.device->setEventReceiver(context.receiver);
		

		/*context.camera = context.smgr->addCameraSceneNodeMaya();
		context.camera->setTarget(core::vector3df(2397*2,643*2,2700*2));
		context.camera->setFarValue(42000.0f);*/

		//context.receiver = new RenderEventReceiver(context);
		//context.device->setEventReceiver(context.receiver);
		
		/*context.camera = context.smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
		context.camera->setPosition(core::vector3df(50,50,-60));
		context.camera->setTarget(core::vector3df(-70,30,-60));*/
		//context.camera->setInputReceiverEnabled(false);

		
		//animation
		/*scene::ISceneNodeAnimator* anim = context.smgr->createFlyCircleAnimator(core::vector3df(0,15,0), 30.0f);
		context.camera->addAnimator(anim);
		anim->drop();*/
		

		//textures
		context.env->addImage(context.driver->getTexture("./media/irrlichtlogo2.png"),core::position2d<s32>(10,10));
		/*context.cube = context.smgr->addCubeSceneNode(10);
		context.cube->setMaterialTexture(0, context.driver->getTexture("media/wall.bmp"));
		context.cube->setMaterialTexture(1, context.driver->getTexture("media/water.jpg"));
		context.cube->setMaterialFlag( video::EMF_LIGHTING, false );
		context.cube->setMaterialType( video::EMT_REFLECTION_2_LAYER );*/

		//context.driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
		// add terrain scene node
		context.terrain = context.smgr->addTerrainSceneNode("./media/terrain-heightmap2.bmp",
			0,					// parent node
			-1,					// node id
			core::vector3df(0.f, 0.f, 0.f),		// position
			core::vector3df(0.f, 0.f, 0.f),		// rotation
			core::vector3df(40.f, 4.4f, 40.f),	// scale
			video::SColor ( 255, 255, 255, 255 ),	// vertexColor
			5,					// maxLOD
			scene::ETPS_17,				// patchSize
			4					// smoothFactor
			);

		context.terrain->setMaterialFlag(video::EMF_LIGHTING, false);

		context.terrain->setMaterialTexture(0,context.driver->getTexture("./media/grass.bmp"));
		context.terrain->setMaterialTexture(1,context.driver->getTexture("./media/grass-detail.bmp"));
		
		context.terrain->setMaterialType(video::EMT_DETAIL_MAP);


		context.terrain->scaleTexture(20.0f, 20.0f);

		//为地形创建一个三角型碰撞检测
		ITriangleSelector* selector = context.smgr->createTerrainTriangleSelector(context.terrain, 0);
		context.terrain->setTriangleSelector(selector);
		selector->drop();
		
		/*
		ITriangleSelector* selector = context.smgr->createTerrainTriangleSelector(context.terrain, 0);
		context.terrain->setTriangleSelector(selector);
		// create collision response animator and attach it to the camera
		scene::ISceneNodeAnimator* anim = context.smgr->createCollisionResponseAnimator(
			selector, context.camera, core::vector3df(60,100,60),
			core::vector3df(0,0,0),
			core::vector3df(0,50,0));

		selector->drop();
		context.camera->addAnimator(anim);
		anim->drop();
		*/


		/*
		context.device->getFileSystem()->addZipFileArchive("./media/map-20kdm2.pk3");

		scene::IAnimatedMesh* q3levelmesh = context.smgr->getMesh("20kdm2.bsp");
		scene::IMeshSceneNode* q3node = 0;

		// The Quake mesh is pickable, but doesn't get highlighted.
		if (q3levelmesh)
			q3node = context.smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, IDFlag_IsPickable);
		scene::ITriangleSelector* selector = 0;

		if (q3node)
		{
			q3node->setPosition(core::vector3df(-1350,-130,-1400));

			selector = context.smgr->createOctreeTriangleSelector(
					q3node->getMesh(), q3node, 128);
			q3node->setTriangleSelector(selector);
			// We're not done with this selector yet, so don't drop it.
		}*/


		scene::IAnimatedMeshSceneNode* node = context.smgr->addAnimatedMeshSceneNode(context.smgr->getMesh("./media/faerie.md2"),
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
		//node->setPosition(core::vector3df(-70,-15,-120)); // Put its feet on the floor.
		node->setPosition(core::vector3df(2700*2,655*2,2600*2)); // Put its feet on the floor.
		node->setScale(core::vector3df(2, 2, 2)); // Make it appear realistically scaled
		node->setMD2Animation(scene::EMAT_POINT);
		node->setAnimationSpeed(20.f);
		video::SMaterial material;
		material.setTexture(0, context.driver->getTexture("./media/faerie2.bmp"));
		material.Lighting = true;
		material.NormalizeNormals = true;
		node->getMaterial(0) = material;
		// Just do the same as we did above.
		selector = context.smgr->createTriangleSelector(node);
		node->setTriangleSelector(selector);
		selector->drop();

		// Add a light, so that the unselected nodes aren't completely dark.
		scene::ILightSceneNode * light = context.smgr->addLightSceneNode(0, core::vector3df(2700*2-60,655*2+100,2600*2+400),
			video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
		light->setID(ID_IsNotPickable); // Make it an invalid target for selection.




		context.collMan = context.smgr->getSceneCollisionManager();

		resize(context.w,context.h);
	}
	void start()
	{
		init();
		Thread::start();
	}
	void run()
	{
		checkDoc();
		context.device->run();
		context.driver->beginScene(true, true, video::SColor(255,23,45,68));
		context.driver->draw3DLine(context.ray.start,context.ray.end,video::SColor(255,0,0,255));
		context.smgr->drawAll();
		if(context.selectedSceneNode)
		{
			context.driver->draw3DTriangle(context.hitTriangle, video::SColor(255,255,255,255));
		}
		context.env->drawAll();
		context.driver->endScene();
		//TRACE("run");
	}
	void stop()
	{
		Thread::stop();
		context.camera->drop();
		context.device->drop();
	}
};







// CRenderView 窗体视图

class CRenderView : public CFormView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	CRenderView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRenderView();

public:
	enum { IDD = IDD_DIALOG_RENDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
private:
	
	SIrrlichtCreationParameters param;
	Context context;
	bool m_initial;
	RenderThread* m_thread;
public:
	void DevRun(const Context& c);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnClose();
private:
	//CWinThread* m_thread;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


