#ifndef _YON_SCENE_CSCENEMANAGER_H_
#define _YON_SCENE_CSCENEMANAGER_H_

#include "ISceneManager.h"
#include "IGeometryFactory.h"

namespace yon{
	namespace scene{

		class CSceneManager : public ISceneManager{
		public:
			CSceneManager();
			virtual ~CSceneManager();

			virtual IModel* addModel(IEntity* entity);
			virtual void render();

			virtual const IGeometryFactory* getGeometryFactory() const;
		private:
			IGeometryFactory* m_geometryFactory;
		};
	}
}
#endif