#ifndef _YON_SCENE_IGEOMETRYFACTORY_H_
#define _YON_SCENE_IGEOMETRYFACTORY_H_

#include "IReferencable.h"
#include "IEntity.h"
#include "dimension3d.h"
#include "dimension2d.h"
#include "SShap.h"
#include "IUnit.h"

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

			virtual Shap2D* createXYRectangle(u32 x0,u32 y0,u32 x1,u32 y1,f32 u0,f32 v0,f32 u1,f32 v1,
				const video::SColor& color=video::COLOR_WHITE) const = 0;

			virtual IUnit* createUnit(Shap2D* shap) = 0;
		};

	}//scene
}//yon
#endif