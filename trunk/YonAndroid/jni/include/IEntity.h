#ifndef _YON_SCENE_IENTITY_H_
#define _YON_SCENE_IENTITY_H_

#include "IReferencable.h"
#include "IUnit.h"

namespace yon{
	namespace scene{

		//相当于irrlicht的IMesh
		class IEntity : public virtual core::IReferencable{
		public:
			virtual u32 getUnitCount() const = 0;
			virtual IUnit* getUnit(u32 index) const = 0;
			//virtual ENUM_DIMEN_MODE getDimenMode() const = 0;
			//virtual ENUM_VERTEX_TYPE getVertexMode() const = 0;
		};
	}//scene
}//yon
#endif