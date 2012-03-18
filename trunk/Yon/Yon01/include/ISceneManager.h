#ifndef _YON_SCENE_ISCENEMANAGER_H_
#define _YON_SCENE_ISCENEMANAGER_H_

#include "IReferencable.h"
#include "IGeometryFactory.h"
#include "IModel.h"

namespace yon{
	namespace scene{

		class ISceneManager : public virtual core::IReferencable{
		public:
			virtual ~ISceneManager() {}

			virtual IModel* addModel(IEntity* entity) = 0;
			virtual void render() = 0;

			virtual const IGeometryFactory* getGeometryFactory() const = 0;
		};

	}//scene
}//yon
#endif