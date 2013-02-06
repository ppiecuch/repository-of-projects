#ifndef _YON_SCENE_ISCENENODE_H_
#define _YON_SCENE_ISCENENODE_H_

#include "IRenderable.h"
#include "IReferencable.h"
#include "yonList.h"
#include "ITexture.h"
//#include "IMaterial.h"
#include "SMaterial.h"
#include "IAnimator.h"
#include "IEntity.h"
#include "ITriangleSelector.h"
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

	class ISceneNode : public virtual core::IReferencable, public virtual IRenderable{
	protected:
		ISceneManager* m_pSceneManager;
		ISceneNode* m_parent;
		core::list<ISceneNode*> m_children;
		core::list<animator::IAnimator*> m_animators;
		ITriangleSelector* m_pTriangleSelector;

		//bool m_bTransformationChanged;
		core::matrix4f m_absoluteTransformation;
		core::vector3df m_relativePosition;
		core::vector3df m_relativeRotation;
		core::vector3df m_relativeScale;
		bool m_bVisible;
		core::stringc m_name;

		u32 m_renderDebugConfig;

		
		
		ISceneNode(ISceneNode* parent,const core::vector3df& pos=core::vector3df(0,0,0),
			const core::vector3df& rot=core::vector3df(0,0,0),
			const core::vector3df& scale=core::vector3df(1,1,1)):
				m_pSceneManager(NULL),m_pTriangleSelector(NULL),m_parent(parent),
				m_relativePosition(pos),m_relativeRotation(rot),m_relativeScale(scale),m_bVisible(true),m_absoluteTransformation(true)
			//m_bTransformationChanged(true),
		{
			if(parent!=NULL){
				m_parent->addChild(this);
			}
			//YON_DEBUG("model:%08X,parent:%08X\r\n",this,m_parent);
		}
		void setSceneManager(ISceneManager* newManager)
		{
			m_pSceneManager=newManager;
			core::list<ISceneNode*>::Iterator it = m_children.begin();
			for (; it != m_children.end(); ++it){
				(*it)->setSceneManager(newManager);
			}
		}
		virtual void addChild(ISceneNode* child)
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
		virtual bool removeChild(ISceneNode* child)
		{
			core::list<ISceneNode*>::Iterator it = m_children.begin();
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
			core::list<ISceneNode*>::Iterator it = m_children.begin();
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

		virtual ~ISceneNode()
		{
			// delete all children
			clearChildren();
			//YON_DEBUG("clearChildren\n");
			clearAnimators();

			if (m_pTriangleSelector)
				m_pTriangleSelector->drop();
		}

		void setRenderDebugConfig(MASK_RENDER_DEBUG_CONFIG config,bool on){
			m_renderDebugConfig = (m_renderDebugConfig & (~config)) |((((u32)!on)-1) & config);
		}

		//! Returns the parent of this scene node
		/** \return A pointer to the parent. */
		scene::ISceneNode* getParent() const
		{
			return m_parent;
		}
		//! Returns a const reference to the list of all children.
		/** \return The list of all children of this node. */
		const core::list<ISceneNode*>& getChildren() const
		{
			return m_children;
		}


		//! Returns the name of the node.
		/** \return Name as character string. */
		virtual const c8* getName() const
		{
			return m_name.c_str();
		}

		//! Sets the name of the node.
		/** \param name New name of the scene node. */
		virtual void setName(const c8* name)
		{
			m_name = name;
			//if(m_name=="weapon")
			//	YON_DEBUG("%s\r\n",m_parent->getName());
		}


		//! Sets the name of the node.
		/** \param name New name of the scene node. */
		virtual void setName(const core::stringc& name)
		{
			m_name = name;
		}

		virtual void setPosition(const core::vector3df& pos){
			m_relativePosition=pos;
			//m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getPosition() const{
			return m_relativePosition;
		}
		//! Gets the absolute position of the node in world coordinates.
		/** If you want the position of the node relative to its parent,
		use getPosition() instead.
		\return The current absolute position of the scene node. */
		virtual core::vector3df getAbsolutePosition()
		{
			return m_absoluteTransformation.getTranslation();
			//return getAbsoluteTransformation().getTranslation();
		}

		virtual void setRotation(const core::vector3df& rotation){
			/*if(m_name=="Bip01_Pelvis")
			{
				YON_DEBUG("Bip01_Pelvis.rotation:\r\n");
				rotation.print();
			}*/
			m_relativeRotation=rotation;
			//m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getRotation() const{
			return m_relativeRotation;
		}

		virtual void setScale(const core::vector3df& scale){
			m_relativeScale=scale;
			//m_bTransformationChanged=true;
		}
		virtual const core::vector3df& getScale() const{
			return m_relativeScale;
		}

		//! Get the absolute transformation of the node. Is recalculated every OnAnimate()-call.
		//! \return The absolute transformation matrix.
		virtual const core::matrix4f& getAbsoluteTransformation() const
		{
			return m_absoluteTransformation;
		}

		virtual const core::matrix4f getRelativeTransformation()
		{
			//if(m_bTransformationChanged){
			core::matrix4f mat(true);
			mat.setRotationDegrees(m_relativeRotation); 
			mat.setTranslation(m_relativePosition);
			if (m_relativeScale != core::IDENTITY_VECTOR3DF)
			{
				mat.scale(m_relativeScale);
			}
			//if(m_name=="hp_hero1_attach")
				//mat.print();
			//m_bTransformationChanged=false;
			//}
			return mat;
			/*core::matrix4f mat;
			mat.setRotationDegrees(m_relativeRotation);
			mat.setTranslation(m_relativePosition);

			if (m_relativeScale != core::IDENTITY_VECTOR3DF)
			{
				mat.scale(m_relativeScale);
			}

			return mat;*/
		}

		/*
		//deprecated
		virtual void setTransformation(const core::matrix4f& m)
		{
			m_transformation=m;
			//TODO
			//m_rotation=m.getRotation();
			m_position=m.getTranslation();
			//m_scale=m.getScale();
		}*/

		virtual bool isVisible() const{
			return m_bVisible;
		}
		virtual void setVisible(bool on){
			m_bVisible=on;
		}
		
		//! Changes the parent of the scene node.
		virtual void setParent(ISceneNode* newParent)
		{
			grab();
			removeFromParent();

			m_parent = newParent;
			//YON_DEBUG("setParent,model:%s,parent:%s\r\n",m_name.c_str(),m_parent->getName());

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
				core::list<ISceneNode*>::Iterator it = m_children.begin();
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

		//! Returns the triangle selector attached to this scene node.
		/** The Selector can be used by the engine for doing collision
		detection. You can create a TriangleSelector with
		ISceneManager::createTriangleSelector() or
		ISceneManager::createOctreeTriangleSelector and set it with
		ISceneNode::setTriangleSelector(). If a scene node got no triangle
		selector, but collision tests should be done with it, a triangle
		selector is created using the bounding box of the scene node.
		\return A pointer to the TriangleSelector or 0, if there
		is none. */
		virtual ITriangleSelector* getTriangleSelector() const
		{
			return m_pTriangleSelector;
		}


		//! Sets the triangle selector of the scene node.
		/** The Selector can be used by the engine for doing collision
		detection. You can create a TriangleSelector with
		ISceneManager::createTriangleSelector() or
		ISceneManager::createOctreeTriangleSelector(). Some nodes may
		create their own selector by default, so it would be good to
		check if there is already a selector in this node by calling
		ISceneNode::getTriangleSelector().
		\param selector New triangle selector for this scene node. */
		virtual void setTriangleSelector(ITriangleSelector* selector)
		{
			if(selector)
				selector->grab();
			if (m_pTriangleSelector)
				m_pTriangleSelector->drop();
			m_pTriangleSelector = selector;
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
				updateAbsolutePosition();

				core::list<ISceneNode*>::Iterator mit = m_children.begin();
				for (; mit != m_children.end(); ++mit)
				{
					(*mit)->onAnimate(timeMs);
				}
			}
		}

		//TODO ¸ÄÎªupdateAbsolutePositionIfNecessary
		//! Updates the absolute position based on the relative and the parents position
		/** Note: This does not recursively update the parents absolute positions, so if you have a deeper
		hierarchy you might want to update the parents first.*/
		virtual void updateAbsolutePosition()
		{
			/*if(m_name=="weapon")
			{
				const core::vector3df& v=m_parent->getAbsoluteTransformation().getTranslation();
				YON_DEBUG("%s:%.2f,%.2f,%.2f\r\n",m_parent->getName(),v.x,v.y,v.z);
			}*/
			if (m_parent)
			{
				m_absoluteTransformation =m_parent->getAbsoluteTransformation() * getRelativeTransformation();
			}
			else
				m_absoluteTransformation = getRelativeTransformation();
		}

		virtual void grabAllTextures(){
			for (u32 i=0; i<getMaterialCount(); ++i)
			{
				for(u32 j=0;j<video::MATERIAL_MAX_TEXTURES;++j)
				{
					video::ITexture* texture=getMaterial(i).getTexture(j);
					if(texture)
						texture->grabFromDriver();
				}
			}
		}

		virtual void dropAllTextures(){
			for (u32 i=0; i<getMaterialCount(); ++i)
			{
				for(u32 j=0;j<video::MATERIAL_MAX_TEXTURES;++j)
				{
					video::ITexture* texture=getMaterial(i).getTexture(j);
					if(texture)
						texture->dropFromDriver();
				}
			}
		}

	};
}
}
#endif