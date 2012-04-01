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

			//不带z坐标,即2DVertex
			virtual IEntity* createXYPlane2D(const core::dimension2df& size=core::dimension2df(1,1)) const = 0;
			//带z坐标,即3DVertex
			virtual IEntity* createXYPlane(const core::dimension2df& size=core::dimension2df(1,1),f32 z=0) const = 0;
			virtual IEntity* createCube(const core::dimension3df& size=core::dimension3df(1,1,1)) const = 0;
			virtual IEntity* createSphere(f32 radius=1.0f,u32 hSteps=8,u32 vSteps=8) const = 0;
		};

	}//scene
}//yon
#endif