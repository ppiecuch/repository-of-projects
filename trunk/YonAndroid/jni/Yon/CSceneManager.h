#ifndef _YON_SCENE_CSCENEMANAGER_H_
#define _YON_SCENE_CSCENEMANAGER_H_

#include "ISceneManager.h"
#include "IModel.h"
#include "IGeometryFactory.h"
#include "yonArray.h"

namespace yon{
	namespace scene{

		class CSceneManager : public ISceneManager, public IModel{
		protected:
			core::array<camera::ICamera*> m_cameras;
			camera::ICamera* m_activeCamera;
		public:
			CSceneManager();
			virtual ~CSceneManager();

			virtual IModel* addModel(IEntity* entity);
			virtual camera::ICamera* addCamera(const core::vector3df& pos,const core::vector3df& up,
				const core::vector3df& lookat,bool makeActive);
			virtual void setActiveCamera(camera::ICamera* camera);

			virtual void render(video::IVideoDriver* driver);

			virtual IGeometryFactory* getGeometryFactory() const;

			virtual animator::IAnimatorFactory* getAnimatorFactory() const;

			virtual void onResize(const core::dimension2du& size);

			virtual bool postEventFromUser(const event::SEvent& evt);
		private:
			IGeometryFactory* m_geometryFactory;
			animator::IAnimatorFactory* m_pAnimatorFactory;
		};
	}
}
#endif