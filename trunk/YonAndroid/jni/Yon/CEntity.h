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
				clear();
			}
			virtual u32 getUnitCount() const{
				return m_units.size();
			}
			virtual IUnit* getUnit(u32 index) const{
				return m_units[index];
			}
			virtual void add(IUnit* unit);
			//��ӵ�һ�����Unit
			virtual void add(video::ITexture* texture,video::ENUM_MATERIAL_TYPE materialType,IShap* shap);
			/*virtual ENUM_VERTEX_TYPE getVertexMode() const{
				return m_mode;
			}*/

			virtual void clear(){
				for(u32 i=0;i<m_units.size();++i){
					m_units[i]->drop();
				}
			}
		private:
			core::array<IUnit*> m_units;

			//���������texture/materialType��Ӧ��unit������֮�����򷵻�NULL
			IUnit* getUnit(video::ITexture* texture,video::ENUM_MATERIAL_TYPE materialType);
		};
	}
}
#endif