#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IRenderable.h"
#include "IGeometryFactory.h"
#include "IModel.h"
#include "dimension2d.h"
#include "ICamera.h"
#include "IEventReceiver.h"
#include "IAnimatorFactory.h"

namespace yon{
	namespace scene{

		class ISceneManager : public virtual core::IRenderable{
		public:
			virtual ~ISceneManager() {}

			virtual IModel* addModel(IEntity* entity) = 0;
			virtual camera::ICamera* addCamera(const core::vector3df& pos = core::vector3df(0,-1,0),
				const core::vector3df& up=core::vector3df(0,1,0),
				const core::vector3df& lookat = core::vector3df(0,0,100),bool makeActive=true) = 0;
			virtual void setActiveCamera(camera::ICamera* camera) = 0;

			virtual IGeometryFactory* getGeometryFactory() const = 0;

			virtual animator::IAnimatorFactory* getAnimatorFactory() const = 0;

			//TODO protected
			virtual void onResize(const core::dimension2du& size) = 0;

			virtual bool postEventFromUser(const event::SEvent& evt) = 0;
		};

	}//scene
}//yon
#endif