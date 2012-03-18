#ifndef _YON_SCENE_CENTITY_H_
#define _YON_SCENE_CENTITY_H_

#include "IEntity.h"
#include "IUnit.h"
#include "yonArray.h"

namespace yon{
	namespace scene{

		class CEntity : public IEntity{
		public:
			virtual ~CEntity(){}
			virtual u32 getUnitCount() const{
				return m_units.size();
			}
			virtual IUnit* getUnit(u32 index) const{
				return m_units[index];
			}
			virtual void addUnit(IUnit* unit){
				if(unit!=NULL){
					//TODO ������yonArray.reallocate�е�delete[] temp����ͻ?
					unit->grab();
					m_units.push(unit);
				}
			}
		private:
			core::array<IUnit*> m_units;
		};
	}
}
#endif