#ifndef _YON_SCENE_IMODEL_H_
#define _YON_SCENE_IMODEL_H_

#include "IRenderable.h"
#include "IReferencable.h"
#include "yonList.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IAnimator.h"
#include "IEntity.h"
//#include "ISceneManager.h"

#include "ILogger.h"

namespace yon{
namespace scene{

	class ISceneManager;

	class IModel : public virtual core::IReferencable, public virtual IRenderable{
	protected:
		ISceneManager* m_pSceneManager;
		IModel* m_parent;
		core::list<IModel*> m_children;
		core::list<animator::IAnimator*> m_animators;

		bool m_bTransformationChanged;
		core::matrix4f m_transformation;
		core::vector3df m_position;
		core::vector3df m_rotation;
		core::vector3df m_scale;
		bool m_bVisible;

		
		
		IModel(IModel* parent,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)):
			m_pSceneManager(NULL),m_parent(parent),m_position(pos),m_rotation(rot),m_scale(scale),
			m_bTransformationChanged(true),m_bVisible(true)
		{
			if(parent!=NULL){
				m_parent->addChild(this);
			}
		}
		void setSceneManager(ISceneManager* newManager)
		{
			m_pSceneManager=newManager;
			core::list<IModel*>::Iterator it = m_children.begin();
			for (; it != m_children.end(); ++it){
				(*it)->setSceneManager(newManager);
			}
		}
		virtual void addChild(IModel* child)
		{
			if (child && (child != this))
			{
				if (m_pSceneManager != child->m_pSceneManager)
					child->setSceneManager(m_pSceneManager);

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

		virtual void setPosition(const core::vector3df& pos){
			m_position=pos;
			m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getPosition() const{
			return m_position;
		}
		//! Gets the absolute position of the node in world coordinates.
		virtual core::vector3df getAbsolutePosition()
		{
			//return AbsoluteTransformation.getTranslation();
			return getAbsoluteTransformation().getTranslation();
		}

		virtual void setRotation(const core::vector3df& rotation){
			m_rotation=rotation;
			m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getRotation() const{
			return m_rotation;
		}

		virtual void setScale(const core::vector3df& scale){
			m_scale=scale;
			m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getScale() const{
			return m_scale;
		}

		virtual const core::matrix4f& getAbsoluteTransformation()
		{
			return getRelativeTransformation();
		}

		virtual const core::matrix4f& getRelativeTransformation()
		{
			if(m_bTransformationChanged){
				m_transformation.makeIdentity();
				m_transformation.setRotationDegrees(m_rotation); 
				m_transformation.setTranslation(m_position);
				if (m_scale != core::IDENTITY_VECTOR3DF)
				{
					m_transformation.scale(m_scale);
				}
				m_bTransformationChanged=false;
			}
			return m_transformation;
		}

		virtual void setTransformation(const core::matrix4f& m)
		{
			m_transformation=m;
			//TODO
			//m_rotation=m.getRotation();
			m_position=m.getTranslation();
			//m_scale=m.getScale();
		}

		virtual bool isVisible() const{
			return m_bVisible;
		}
		virtual void setVisible(bool on){
			m_bVisible=on;
		}

		virtual bool removeFromParent(){
			if(m_parent){
				return m_parent->removeChild(this);
			}
			return false;
		}

		//! This method is called just before the rendering process of the whole scene.
		virtual void onRegisterForRender(){
			if(m_bVisible)
			{
				core::list<IModel*>::Iterator it = m_children.begin();
				for (; it != m_children.end(); ++it)
					(*it)->onRegisterForRender();
			}
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

		virtual IEntity* getEntity(){
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

		virtual void onAnimate(u32 timeMs)
		{
			if(m_bVisible)
			{
				core::list<animator::IAnimator*>::Iterator ait=m_animators.begin();
				for (; ait != m_animators.end(); ++ait)
					(*ait)->animateNode(this,timeMs);

				core::list<IModel*>::Iterator mit = m_children.begin();
				for (; mit != m_children.end(); ++mit)
				{
					(*mit)->onAnimate(timeMs);
				}
			}
		}

	};
}
}
#endif