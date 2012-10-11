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

	class IModel;
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

		//TODO ÃÌº”parent,pos,rotation,scale
		virtual IModel* addModel(IEntity* entity) = 0;
		//TODO ÃÌº”parent,pos,rotation,scale
		virtual IAnimatedSceneNode* addAnimatedSceneNode(IAnimatedEntity* entity) = 0;

		virtual IModel* getRootModel() = 0;

		virtual void clearModels() = 0;
		virtual camera::ICamera* addCamera(camera::ENUM_CAMERA_TYPE cameraType=camera::ENUM_CAMERA_TYPE_ORTHO,IModel* parent=NULL,
			const core::vector3df& pos = core::vector3df(0,-1,0),
			const core::vector3df& up=core::vector3df(0,1,0),
			const core::vector3df& lookat = core::vector3df(0,0,100),bool makeActive=true) = 0;
		virtual camera::ICamera* addCameraFPS(IModel* parent=NULL,f32 moveSpeed=0.1f,f32 rotateSpeed=100.f, event::SKeyMap* keyMapArray=0,s32 keyMapSize=0, 
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
		virtual bool registerForRender(IModel* model,ENUM_SCENE_PASS pass=ENUM_SCENE_PASS_AUTOMATIC) = 0;

		//! Get current render pass.
		virtual ENUM_SCENE_PASS getRenderingPass() const = 0;

		//TODO protected
		//virtual void onResize(const core::dimension2du& size) = 0;

		//virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		virtual bool onEvent(const event::SEvent& evt) = 0;

		//! Check if node is culled in current view frustum
		// return True if node is not visible in the current scene, else false.
		virtual bool isCulled(const IModel* model) const{
			//TODO ¡Ÿ ±
			return false;
		}

		virtual terrain::ITerrainModel* addTerrainModel(IModel* parent=NULL,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)) = 0;

		virtual water::IWaterModel* addWaterModel(s32 xsize,s32 zsize, IModel* parent=NULL,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)) = 0;

		virtual IModel* addSkyBoxModel(video::ITexture* front, video::ITexture* back, video::ITexture* left,
			video::ITexture* right, video::ITexture* top, video::ITexture* bottom,IModel* parent=NULL) = 0;

		virtual IAnimatedEntity* getEntity(const io::path& filename) = 0;
		virtual IAnimatedEntity* getEntity(io::IReadStream* file) = 0;
			
		//! Get a skinned entity, which is not available as header-only code
		virtual ISkinnedEntity* createSkinnedEntity() = 0;
	};

}
}
#endif