#include "CModel.h"

namespace yon{
	namespace scene{

		CModel::CModel(IEntity* entity,IModel* parent):m_entity(NULL),IModel(parent){
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