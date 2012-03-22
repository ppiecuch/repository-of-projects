#ifndef _YON_SCENE_CSCENEMANAGER_H_
#define _YON_SCENE_CSCENEMANAGER_H_

#include "ISceneManager.h"
#include "IModel.h"
#include "IGeometryFactory.h"

namespace yon{
	namespace scene{

		class CSceneManager : public ISceneManager, public IModel{
		public:
			CSceneManager();
			virtual ~CSceneManager();

			virtual IModel* addModel(IEntity* entity);
			virtual void render(video::IVideoDriver* driver);

			virtual const IGeometryFactory* getGeometryFactory() const;
		private:
			IGeometryFactory* m_geometryFactory;
		};
	}
}
#endif