#ifndef _YON_SCENE_IGEOMETRYFACTORY_H_
#define _YON_SCENE_IGEOMETRYFACTORY_H_

#include "IReferencable.h"
#include "IEntity.h"
#include "dimension3d.h"
#include "dimension2d.h"

namespace yon{
	namespace scene{

		class IGeometryFactory : public core::IReferencable{
		public:

			virtual IEntity* createCube(const core::dimension3df& size=core::dimension3df(1,1,1)) const = 0;
			virtual IEntity* createSphere(f32 radius=1.0f,const core::dimension2du& detail=core::dimension2du(8,8)) const = 0;
		};

	}//scene
}//yon
#endif