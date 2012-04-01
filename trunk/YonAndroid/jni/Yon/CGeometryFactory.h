#ifndef _YON_SCENE_CGEOMETRYFACTORY_H_
#define _YON_SCENE_CGEOMETRYFACTORY_H_

#include "IGeometryFactory.h"

namespace yon{
	namespace scene{

		class CGeometryFactory : public IGeometryFactory{
		public:

			virtual IEntity* createXYPlane2D(const core::dimension2df& size) const;
			virtual IEntity* createXYPlane(const core::dimension2df& size,f32 z) const;
			virtual IEntity* createCube(const core::dimension3df& size) const;
			virtual IEntity* createSphere(f32 radius,u32 hSteps,u32 vSteps) const;
		};
	}
}
#endif