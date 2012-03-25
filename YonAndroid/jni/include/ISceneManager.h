#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IRenderable.h"
#include "IGeometryFactory.h"
#include "IModel.h"
#include "dimension2d.h"
#include "ICamera.h"

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

			virtual const IGeometryFactory* getGeometryFactory() const = 0;

			//TODO protected
			virtual void onResize(const core::dimension2du& size) = 0;
		};

	}//scene
}//yon
#endif