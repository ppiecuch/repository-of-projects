#ifndef _YON_SCENE_IMODEL_H_
#define _YON_SCENE_IMODEL_H_

#include "IRenderable.h"
#include "IReferencable.h"
#include "yonList.h"
#include "ITexture.h"
//#include "IMaterial.h"
#include "SMaterial.h"
#include "IAnimator.h"
#include "IEntity.h"
//#include "ISceneManager.h"

#include "ILogger.h"

namespace yon{
namespace scene{

	enum MASK_RENDER_DEBUG_CONFIG{
		MASK_RENDER_DEBUG_CONFIG_OFF = 0,					//! No Debug Data ( Default )
		MASK_RENDER_DEBUG_CONFIG_BBOX = 1,					//! Show Bounding Boxes of SceneNode
		MASK_RENDER_DEBUG_CONFIG_NORMAL = 2,				//! Show Vertex Normals
		MASK_RENDER_DEBUG_CONFIG_SKELETON = 4,				//! Shows Skeleton/Tags
		MASK_RENDER_DEBUG_CONFIG_WIREFRAME = 8,				//! Overlays Mesh Wireframe
		MASK_RENDER_DEBUG_CONFIG_HALF_TRANSPARENCY = 16,	//! Temporary use transparency Material Type
		MASK_RENDER_DEBUG_CONFIG_BBOX_UNITS = 32,			//! Show Bounding Boxes of all MeshBuffers
		MASK_RENDER_DEBUG_CONFIG_FULL = 0xFFFFFFFF
	};

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

		u32 m_renderDebugConfig;

		
		
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
		//debug
		//core::stringc debugName;

		virtual ~IModel()
		{
			// delete all children
			clearChildren();
			//Logger->debug("clearChildren\n");
			clearAnimators();
		}

		void setRenderDebugConfig(MASK_RENDER_DEBUG_CONFIG config,bool on){
			m_renderDebugConfig = (m_renderDebugConfig & (~config)) |((((u32)!on)-1) & config);
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
		
		//! Changes the parent of the scene node.
		virtual void setParent(IModel* newParent)
		{
			grab();
			removeFromParent();

			m_parent = newParent;

			if (m_parent)
				m_parent->addChild(this);

			drop();
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

		virtual void setMaterialTexture(u32 textureLayer, video::ITexture* texture){
			if (textureLayer>=video::MATERIAL_MAX_TEXTURES)
				return;

			for (u32 i=0; i<getMaterialCount(); ++i)
			{
				YON_DEBUG_BREAK_IF(&getMaterial(i)==&video::DEFAULT_MATERIAL);
				getMaterial(i).setTexture(textureLayer, texture);
			}
		}

		virtual u32 getMaterialCount() const
		{
			return 0;
		}

		//virtual video::IMaterial* getMaterial(u32 num) const{
		//	return NULL;
		//}
		virtual video::SMaterial& getMaterial(u32 num){
			return video::DEFAULT_MATERIAL;
		}

		virtual IEntity* getEntity(){
			return NULL;
		}

		void setMaterialType(video::ENUM_MATERIAL_TYPE newType)
		{
			for (u32 i=0; i<getMaterialCount(); ++i)
			{
				YON_DEBUG_BREAK_IF(&getMaterial(i)==&video::DEFAULT_MATERIAL);
				getMaterial(i).MaterialType=newType;
			}
		}

		virtual void addAnimator(animator::IAnimator* animator)
		{
			if (animator)
			{
				m_animators.push_back(animator);
				animator->grab();
			}
		}

		//! OnAnimate() is called just before rendering the whole scene.
		/** Nodes may calculate or store animations here, and may do other useful things,
		depending on what they are. Also, OnAnimate() should be called for all
		child scene nodes here. This method will be called once per frame, independent
		of whether the scene node is visible or not.
		\param timeMs Current time in milliseconds. */
		virtual void onAnimate(u32 timeMs)
		{
			if(m_bVisible)
			{
				core::list<animator::IAnimator*>::Iterator ait=m_animators.begin();
				for (; ait != m_animators.end(); ++ait)
					(*ait)->animateNode(this,timeMs);
					
				// update absolute position
				//updateAbsolutePosition();

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