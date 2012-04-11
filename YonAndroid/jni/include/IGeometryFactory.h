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

			/*//不带z坐标,即2DVertex
			virtual IEntity* createXYPlane2D(const core::dimension2df& size=core::dimension2df(1,1)) const = 0;
			//带z坐标,即3DVertex
			virtual IEntity* createXYPlane(const core::dimension2df& size=core::dimension2df(1,1),f32 z=0) const = 0;
			virtual IEntity* createCube(const core::dimension3df& size=core::dimension3df(1,1,1)) const = 0;*/

			virtual IShap* createTorus(f32 cirRadius=1.0f,f32 orbitRadius=2.0f,u32 cirSteps=8,u32 orbitSteps=8,const video::SColor& color=video::COLOR_WHITE) const = 0;
			virtual IShap* createSphere(f32 radius=1.0f,u32 hSteps=8,u32 vSteps=8) const = 0;
			virtual IShap* createCube(f32 width=1,f32 height=1,f32 depth=1,const video::SColor& color=video::COLOR_WHITE) const = 0;
			virtual IShap* createXYRectangle2D(s32 x0,s32 y0,s32 x1,s32 y1,f32 u0=0,f32 v0=0,f32 u1=1,f32 v1=1,
				const video::SColor& color=video::COLOR_WHITE) const = 0;
			virtual IShap* createXYRectangle3D(s32 x0,s32 y0,s32 x1,s32 y1,f32 z,f32 u0=0,f32 v0=0,f32 u1=1,f32 v1=1,
				const video::SColor& color=video::COLOR_WHITE) const = 0;
			virtual IShap* createXYRectangle2D2T(s32 x0,s32 y0,s32 x1,s32 y1,f32 u0=0,f32 v0=0,f32 u1=1,f32 v1=1,f32 s0=0,f32 t0=0,f32 s1=1,f32 t1=1,
				const video::SColor& color=video::COLOR_WHITE) const = 0;

			virtual IUnit* createUnit(IShap* shap) const = 0;
			virtual IEntity* createEntity(IUnit* unit) const = 0;
		};

	}//scene
}//yon
#endif