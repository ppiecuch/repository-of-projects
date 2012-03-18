#ifndef _YON_SCENE_IMODEL_H_
#define _YON_SCENE_IMODEL_H_

#include "IRenderable.h"
#include "yonList.h"

#include "ILogger.h"

namespace yon{
	namespace scene{

		class IModel : public virtual core::IRenderable{
		protected:
			IModel* m_parent;
			core::list<IModel*> m_children;
			
			IModel(IModel* parent):m_parent(parent){
				if(parent!=NULL){
					m_parent->addChild(this);
				}
			}
			virtual void addChild(IModel* child)
			{
				if (child && (child != this))
				{
					child->grab();
					child->removeFromParent();
					m_children.push_back(child);
					child->m_parent = this;
				}
			}
			virtual bool removeChild(IModel* child)
			{
				core::list<IModel*>::Iterator it = m_children.begin();
				for (; it != m_children.end(); ++it){
					if ((*it) == child)
					{
						(*it)->m_parent = NULL;
						(*it)->drop();
						m_children.erase(it);
						return true;
					}
				}
				return false;
			}
			virtual void clearChildren()
			{
				core::list<IModel*>::Iterator it = m_children.begin();
				for (; it != m_children.end(); ++it)
				{
					(*it)->m_parent = NULL;
					(*it)->drop();
				}
				m_children.clear();
			}
		public:
			virtual ~IModel()
			{
				// delete all children
				clearChildren();
				//Logger->debug("clearChildren\n");
			}
			virtual bool removeFromParent(){
				if(m_parent){
					return m_parent->removeChild(this);
				}
				return false;
			}
		};
	}//scene
}//yon
#endif