#include "CModel.h"

namespace yon{
	namespace scene{

		CModel::CModel(IEntity* entity):m_entity(NULL){
			setEntity(entity);
		}


		void CModel::render(){}

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