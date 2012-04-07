#ifndef _YON_SCENE_IMODEL_H_
#define _YON_SCENE_IMODEL_H_

#include "IRenderable.h"
#include "yonList.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IAnimator.h"

#include "ILogger.h"

namespace yon{
	namespace scene{

		class IModel : public virtual core::IRenderable{
		protected:
			IModel* m_parent;
			core::list<IModel*> m_children;
			core::list<animator::IAnimator*> m_animators;

			
			
			IModel(IModel* parent,const core::vector3df& pos=core::vector3df(0,0,0),
				const core::vector3df& rot=core::vector3df(0,0,0),
				const core::vector3df& scale=core::vector3df(1,1,1)):
				m_parent(parent),IRenderable(pos,rot,scale)
			{
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
			virtual void clearAnimators()
			{
				core::list<animator::IAnimator*>::Iterator it = m_animators.begin();
				for (; it != m_animators.end(); ++it)
					(*it)->drop();

				m_animators.clear();
			}
		public:
			virtual ~IModel()
			{
				// delete all children
				clearChildren();
				//Logger->debug("clearChildren\n");
				clearAnimators();
			}
			virtual bool removeFromParent(){
				if(m_parent){
					return m_parent->removeChild(this);
				}
				return false;
			}

			virtual void setMaterialTexture(u32 textureLayer, video::ITexture* texture) const{
				if (textureLayer>=video::MATERIAL_MAX_TEXTURES)
					return;

				for (u32 i=0; i<getMaterialCount(); ++i)
					getMaterial(i)->setTexture(textureLayer, texture);
			}

			virtual u32 getMaterialCount() const
			{
				return 0;
			}

			virtual video::IMaterial* getMaterial(u32 num) const{
				return NULL;
			}

			void setMaterialType(video::ENUM_MATERIAL_TYPE newType)
			{
				for (u32 i=0; i<getMaterialCount(); ++i)
					getMaterial(i)->setMaterialType(newType);
			}

			virtual void addAnimator(animator::IAnimator* animator)
			{
				if (animator)
				{
					m_animators.push_back(animator);
					animator->grab();
				}
			}

		};
	}//scene
}//yon
#endif