#include "CModel.h"

namespace yon{
	namespace scene{

		CModel::CModel(IModel* parent,IEntity* entity,
			const core::vector3df& pos,const core::vector3df& rot,const core::vector3df& scale):
			m_entity(NULL),IModel(parent,pos,rot,scale){
			setEntity(entity);
		}
		CModel::~CModel(){
			if(m_entity){
				m_entity->drop();
			}
		}


		void CModel::render(video::IVideoDriver* driver){
			for(u32 i=0;i<m_entity->getUnitCount();++i){
				driver->drawUnit(m_entity->getUnit(i));
			}
		}

		void CModel::setEntity(IEntity* entity){
			
			if(entity==NULL)
				return;
	
			entity->grab();
			if(m_entity!=NULL){
				m_entity->drop();
			}
			m_entity=entity;
		}

	}
}