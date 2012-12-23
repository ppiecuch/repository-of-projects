#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IRenderable.h"
#include "IGeometryFactory.h"
#include "dimension2d.h"
#include "ICamera.h"
#include "IEventReceiver.h"
#include "IAnimatorFactory.h"
#include "SKeyMap.h"
#include "IStream.h"
#include "ISkinnedEntity.h"

namespace yon{
	namespace video{
		class IVideoDriver;
	}
namespace scene{

	//! Enumeration for render passes.
	enum ENUM_SCENE_PASS
	{
		//! No pass currently active
		ENUM_SCENE_PASS_NONE = 0,

		//! Camera pass. The active view is set up here. The very first pass.
		ENUM_SCENE_PASS_CAMERA,

		//! Sky box pass.
		ENUM_SCENE_PASS_SKYBOX,

		//! Solid scene models
		ENUM_SCENE_PASS_SOLID,

		//! Transparent scene models
		ENUM_SCENE_PASS_TRANSPARENT,

		//! Transparent effect scene models
		ENUM_SCENE_PASS_EFFECT,

		ENUM_SCENE_PASS_AUTOMATIC,

		ENUM_SCENE_PASS_COUNT
	};

	class ISceneNode;
	namespace terrain{
		class ITerrainModel;
	}
	namespace water{
		class IWaterModel;
	}

	class IAnimatedEntity;
	class IAnimatedSceneNode;

	//class ISceneManager : public virtual core::IRenderable{
	class ISceneManager : public virtual core::IReferencable,public virtual IRenderable{
	public:
		virtual ~ISceneManager() {}

		virtual video::IVideoDriver* getVideoDriver() const= 0;

		//TODO ÃÌº”parent,pos,rotation,scale
		virtual ISceneNode* addSceneNode(IEntity* entity) = 0;
		//TODO ÃÌº”parent,pos,rotation,scale
		virtual IAnimatedSceneNode* addAnimatedSceneNode(IAnimatedEntity* entity,ISceneNode* parent=NULL) = 0;

		virtual ISceneNode* getRootSceneNode() = 0;

		virtual void clearSceneNodes() = 0;
		virtual camera::ICamera* addCamera(camera::ENUM_CAMERA_TYPE cameraType=camera::ENUM_CAMERA_TYPE_ORTHO,ISceneNode* parent=NULL,
			const core::vector3df& pos = core::vector3df(0,-1,0),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,100),bool makeActive=true) = 0;
		virtual camera::ICamera* addCameraFPS(ISceneNode* parent=NULL,f32 moveSpeed=0.1f,f32 rotateSpeed=100.f, event::SKeyMap* keyMapArray=0,s32 keyMapSize=0, 
			const core::vector3df& pos = core::vector3df(0,0,-1),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,100),
			bool makeActive=true) = 0;
		//virtual void setActiveCamera(camera::ICamera* camera) = 0;
		//virtual camera::ICamera* getActiveCamera() const = 0;
		virtual void setViewingCamera(camera::ICamera* camera) = 0;
		virtual camera::ICamera* getViewingCamera() const = 0;

		virtual void setLogisticCamera(camera::ICamera* camera) = 0;
		virtual camera::ICamera* getLogisticCamera() const = 0;
			
		virtual IGeometryFactory* getGeometryFactory() const = 0;

		virtual animator::IAnimatorFactory* getAnimatorFactory() const = 0;

		//! Registers a model for rendering it at a specific time.
		//return the model will be render(passed culling)
		virtual bool registerForRender(ISceneNode* model,ENUM_SCENE_PASS pass=ENUM_SCENE_PASS_AUTOMATIC) = 0;

		//! Get current render pass.
		virtual ENUM_SCENE_PASS getRenderingPass() const = 0;

		//TODO protected
		//virtual void onResize(const core::dimension2du& size) = 0;

		//virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		virtual bool onEvent(const event::SEvent& evt) = 0;

		//! Check if node is culled in current view frustum
		// return True if node is not visible in the current scene, else false.
		virtual bool isCulled(const ISceneNode* model) const{
			//TODO ¡Ÿ ±
			return false;
		}

		//! Creates a simple ITriangleSelector, based on an animated entity scene node.
		/** Details of the mesh associated with the node will be extracted internally.
		Call ITriangleSelector::update() to have the triangle selector updated based
		on the current frame of the animated entity scene node.
		\param node The animated mesh scene node from which to build the selector
		*/
		virtual ITriangleSelector* createTriangleSelector(IAnimatedSceneNode* node) = 0;

		virtual terrain::ITerrainModel* addTerrainModel(ISceneNode* parent=NULL,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)) = 0;

		virtual water::IWaterModel* addWaterModel(s32 xsize,s32 zsize, ISceneNode* parent=NULL,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)) = 0;

		virtual ISceneNode* addSkyBoxModel(video::ITexture* front, video::ITexture* back, video::ITexture* left,
			video::ITexture* right, video::ITexture* top, video::ITexture* bottom,ISceneNode* parent=NULL) = 0;

		virtual IAnimatedEntity* getEntity(const io::path& filename) = 0;
		virtual IAnimatedEntity* getEntity(io::IReadStream* file) = 0;
			
		//! Get a skinned entity, which is not available as header-only code
		virtual ISkinnedEntity* createSkinnedEntity() = 0;
	};

}
}
#endif