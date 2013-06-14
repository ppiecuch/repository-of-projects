#ifndef _MUI_LAYERITEM_H_
#define _MUI_LAYERITEM_H_

namespace mui{

	LayerItem::LayerItem():
		m_pLayer(NULL),m_pLayerNode(NULL),m_pSavedLayerNode(NULL)
	{
	}

	LayerItem::~LayerItem(){
	}

	ILayer* LayerItem::getLayer() const{
		return m_pLayer;
	}

	ILayerNode* LayerItem::getLayerNode() const{
		return m_pLayerNode;
	}

	ILayerItem* LayerItem::getLayerItemByPosition(const core::position2di& pos) const;
	const core::recti& LayerItem::getLayerItemCoordinate() const;

	void LayerItem::attach(ILayer* layer, ILayerNode* node){
		m_pLayer=layer;
		m_pLayerNode=node;

		attachToLayerItemNode(node,true);
	}
	void LayerItem::detach();
	void LayerItem::raiseLayerItem();

	void LayerItem::setMaterial(const video::SMaterial& material);

	void LayerItem::addChildItem(LayerItem* item){
		m_children.push_back(item);
		if(m_pLayerNode)
		{
			item->attachToLayerItemNode(m_pLayerNode,false);
		}
	}
	void LayerItem::removeChildItem(LayerItem* item){
		for(u32 i=0;i<m_children.size();++i)
		{
			if(m_children[i]==item)
			{
				m_children.erase(i);
				return;
			}
		}
		YON_DEBUG_BREAK_IF(true);
	}

	void LayerItem::addChildNode(LayerItem* item){
		m_independents.push_back(item);
		if(m_pLayerNode)
		{
			//TODO:疑问:不明为何要使用子节点
			ILayerNode* child = m_pLayerNode->addChild();
			item->attachToLayerItemNode(child, true);
		}
	}
	void LayerItem::removeChildNode(LayerItem* item){
		for(u32 i=0;i<m_independents.size();++i)
		{
			if(m_independents[i]==item)
			{
				m_independents.erase(i);
				return;
			}
		}
		YON_DEBUG_BREAK_IF(true);
	}

	void LayerItem::addWidget(IBaseWidget* widget){
		m_widgets.push_back(widget);
	}

	void LayerItem::removeAllWidgets(){
		detachFromLayerItemNode(false);
		m_widgets.clear();
	}

	void LayerItem::saveLayerItem(){
		m_pSavedLayerNode=m_pLayerNode;
	}
	void LayerItem::restoreLayerItem(){
		m_pLayerNode=m_pSavedLayerNode;
		if(m_pLayerNode)
			attachToLayerItemNode(m_pLayerNode,false);
	}

	//TODO  待理解
	void LayerItem::attachToLayerItemNode(ILayerNode* node, bool deep){
		YON_DEBUG_BREAK_IF(node==NULL);

		m_pLayerNode=node;

		for(u32 i=0;i<m_widgets.size();++i)
		{
			m_widgets[i]->createDrawItem(m_material,node);
		}

		for(u32 i=0;i<m_children.size();++i)
		{
			m_children[i]->attachToLayerItemNode(node,deep);
		}

		if(deep)
		{
			for(u32 i=0;i<m_independents.size();++i)
			{
				ILayerNode* child=node->addChild();
				m_independents[i]->attachToLayerItemNode(child,deep);
			}
		}
	}

	//TODO 待理解
	void LayerItem::detachFromLayerItemNode(bool deep){
		for(u32 i=0;i<m_children.size();++i)
			m_children[i]->detachFromLayerItemNode(deep);

		if(deep)
		{
			for(u32 i=0;i<m_independents.size();++i)
			{
				ILayerNode* node=m_independents[i]->m_pLayerNode;
				m_independents[i]->detachFromLayerItemNode(deep);
				if(node)
				{
					node->getLayer()->removeChildNode(node);
				}
			}
		}

		if(m_pLayerNode)
		{
			for(u32 i=0;i<m_widgets.size();++i)
			{
				m_widgets[i]->destroyDrawItem();
			}
			m_pLayerNode=NULL;
		}
	}
}
#endif