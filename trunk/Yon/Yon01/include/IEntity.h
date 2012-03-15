#ifndef _YON_SCENE_IENTITY_H_
#define _YON_SCENE_IENTITY_H_

#include "IReferencable.h"
#include "IUnit.h"

namespace yon{
	namespace scene{

		class IEntity : public virtual IReferencable{
		public:
			virtual ~IEntity() = 0;
			virtual u32 getUnitCount() const = 0;
			virtual IUnit* getUnit(u32 index) const = 0;
			virtual void addUnit(const IUnit* unit) = 0;
		};
	}//scene
}//yon
#endif