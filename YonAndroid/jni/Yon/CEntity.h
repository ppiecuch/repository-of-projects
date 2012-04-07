#ifndef _YON_SCENE_CENTITY_H_
#define _YON_SCENE_CENTITY_H_

#include "IEntity.h"
#include "IUnit.h"
#include "yonArray.h"

namespace yon{
	namespace scene{

		class CEntity : public IEntity{
		protected:
			//const ENUM_DIMEN_MODE m_mode;
			//const ENUM_VERTEX_TYPE m_mode;
		public:
			//CEntity(ENUM_VERTEX_TYPE mode=ENUM_VERTEX_TYPE_3V1T1C):m_mode(mode){}
			virtual ~CEntity(){
				for(u32 i=0;i<m_units.size();++i){
					m_units[i]->drop();
				}
			}
			virtual u32 getUnitCount() const{
				return m_units.size();
			}
			virtual IUnit* getUnit(u32 index) const{
				return m_units[index];
			}
			virtual void addUnit(IUnit* unit){
				if(unit!=NULL){
					//TODO 这里与yonArray.reallocate中的delete[] temp不冲突?
					unit->grab();
					m_units.push(unit);
				}
			}
			/*virtual ENUM_VERTEX_TYPE getVertexMode() const{
				return m_mode;
			}*/
		private:
			core::array<IUnit*> m_units;
		};
	}
}
#endif