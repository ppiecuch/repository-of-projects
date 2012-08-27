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
			//core::list<animator::IAnimator*>::Iterator it = m_animators.begin();
			//for (; it != m_animators.end(); ++it)
			//	(*it)->animateNode(this,0);

			driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());
			for(u32 i=0;i<m_entity->getUnitCount();++i){
				driver->setMaterial(m_entity->getUnit(i)->getMaterial());
				driver->drawUnit(m_entity->getUnit(i));
			}
			Logger->debug("render:%s\r\n",debugName.c_str());
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