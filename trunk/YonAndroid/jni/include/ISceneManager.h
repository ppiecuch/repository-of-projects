#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IRenderable.h"
#include "IGeometryFactory.h"
#include "IModel.h"

namespace yon{
	namespace scene{

		class ISceneManager : public virtual core::IRenderable{
		public:
			virtual ~ISceneManager() {}

			virtual IModel* addModel(IEntity* entity) = 0;

			virtual const IGeometryFactory* getGeometryFactory() const = 0;
		};

	}//scene
}//yon
#endif