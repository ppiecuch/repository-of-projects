#include "MUI_LayerNode.h"

namespace mui{

	LayerNode::LayerNode(ILayer* layer, ILayerNode* parent)
		:m_pParent(NULL),m_pLayer(layer),
	{
	}

	LayerNode::~LayerNode(){
		for(u32 i=0;i<m_stardardRenderItems.size();++i)
			delete m_stardardRenderItems[i];
		m_stardardRenderItems.clear();

		for(u32 i=0;i<m_fastRenderItems.size();++i)
			delete m_fastRenderItems[i];
		m_fastRenderItems.clear();

		for(u32 i=0;i<m_children.size();++i)
			delete m_children[i];
		m_children.clear();
	}

	ILayerNode* LayerNode::getParent() const{
		return m_pParent;
	}

	ILayer* LayerNode::getLayer() const{
		return m_pLayer;
	}

	ILayerNode* LayerNode::addChild(){
		LayerNode* node = new LayerNode(m_pLayer, this);
		m_children.push_back(node);

		m_bDirty = true;

		return node;
	}

	void LayerNode::removeChild(ILayerNode* node){
		YON_DEBUG_BREAK_IF(node==NULL);
		for(u32 i=0;i<m_children.size();++i)
		{
			if(m_children[i]==node)
			{
				delete node;
				m_children.erase(i);
				return;
			}
		}
		YON_DEBUG_BREAK_IF(true);
	}

	void LayerNode::raiseChild(ILayerNode* node){
		//TODO
	}

	u32 LayerNode::getChildrenCount() const{
		return m_children.size();
	}

	RenderItem* LayerNode::addToRenderItem(const video::SMaterial& material,bool standard){
		if(standard)
		{
			if(m_stardardRenderItems.empty())
			{
				RenderItem* item = new RenderItem();
				item->setMaterial(material);
				m_stardardRenderItems.push_back(item);
				m_bDirty = false;
				return item;
			}

			u32 index=-1;
			if(m_stardardRenderItems.getLast()->getRequestVertexCount()==0)
			{
				index=m_stardardRenderItems.size()-1;
				for(u32 i=m_stardardRenderItems.size()-1;i>=0;--i)
				{
					if(m_stardardRenderItems[i]->getRequestVertexCount()==0)
					{
						index=i;
						continue;
					}
					if(m_stardardRenderItems[i]->getMaterial()==material)
					{
						index=i;
						break;
					}
				}
				m_stardardRenderItems[index]->setMaterial(material);
				m_bDirty=false;
				return m_stardardRenderItems[index];
			}
			else if(m_stardardRenderItems.getLast()->getMaterial()==material)
			{
				m_bDirty=false;
				return m_stardardRenderItems.getLast();
			}

			RenderItem* item = new RenderItem();
			item->setMaterial(material);
			m_stardardRenderItems.push_back(item);
			m_bDirty = false;
			return item;
		}

		for(u32 i=0;i<m_fastRenderItems.size();++i)
		{
			if(m_fastRenderItems[i]->getMaterial()==material)
			{
				m_bDirty=false;
				return m_fastRenderItems[i];
			}
			if(m_fastRenderItems[i]->getRequestVertexCount()==0)
			{
				m_fastRenderItems[i]->setMaterial(material);
				m_bDirty=false;
				return m_fastRenderItems[i];
			}
		}

		RenderItem* item = new RenderItem();
		item->setMaterial(material);
		m_fastRenderItems.push_back(item);
		m_bDirty = false;
		return item;
	}

	void LayerNode::setDirty(RenderItem* item){
		m_bDirty=true;
		if(item)
			item->setDirty();
	}

	ILayerItem* LayerNode::getLayerItemByPosition(const core::position2di& pos) const{
		//TODO
	}

	//refer to:http://www.ogre3d.org/addonforums/viewtopic.php?f=17&t=13882
	//This function move "empty" (have no vertices to draw) buffers to end of vector. 
	//Then whenever new RenderItem created and last buffer is empty RenderItem can use previously allocated buffer from empty render item instead of creating new one.
	void LayerNode::updateStardardRenderItems(){
		/*
		if (mFirstRenderItems.size() > 1)
		{
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end())
			{
				if ((*iter1)->getNeedVertexCount() == 0 && !(*iter1)->getManualRender() && !(*iter2)->getManualRender())
				{
					RenderItem* tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}

		mOutOfDate = true;
		*/
		//TODO:疑问:如果是将空item移至结尾，那么本质上就是一种排序，但这里只执行N次比较，显然不是正确的排序算法
		//此算法相当于冒泡排序的一个子操作，但如果数组中存在getRequestVertexCount()==0的元素，则肯定会将这类元素中的一个移至数组结尾,
		//不确定一个这样的元素（如果可能的话）是否可以满足使用需求？
		for(u32 i=1;i<m_stardardRenderItems.size();++i)
		{
			if(m_stardardRenderItems[i-1]->getRequestVertexCount()==0)
			{
				RenderItem* tmp=m_stardardRenderItems[i-1];
				m_stardardRenderItems[i-1]=m_stardardRenderItems[i];
				m_stardardRenderItems[i]=tmp;

				m_bDirty=true;
			}
		}
	}

	void LayerNode::renderToTarget(IRenderTarget* target){
		bool needCompression = false;

		//TODO:疑问，不懂？
		for (u32 i=0;i<m_stardardRenderItems.size();++i)
		{
			if (m_stardardRenderItems[i]->isCompression())
			{
				needCompression = true;
				break;
			}
		}

		if (needCompression)
			updateStardardRenderItems();

		for(u32 i=0;i<m_stardardRenderItems.size();++i)
			m_stardardRenderItems[i]->renderToTarget(target);

		for(u32 i=0;i<m_fastRenderItems.size();++i)
			m_fastRenderItems[i]->renderToTarget(target);

		for(u32 i=0;i<m_children.size();++i)
			m_children[i]->renderToTarget(target);

		m_bDirty=false;
	}

	f32 LayerNode::getDepth(){
		return m_fDepth;
	}

	bool LayerNode::isDirty() const{
		/*
		for (VectorRenderItem::const_iterator item = mFirstRenderItems.begin(); item != mFirstRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorRenderItem::const_iterator item = mSecondRenderItems.begin(); item != mSecondRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorILayerNode::const_iterator item = mChildItems.begin(); item != mChildItems.end(); ++item)
		{
			if (static_cast<const LayerNode*>(*item)->isOutOfDate())
				return true;
		}

		return mOutOfDate;
		*/

		for(u32 i=0;i<m_stardardRenderItems.size();++i)
			if(m_stardardRenderItems[i]->isDirty())
				return true;

		for(u32 i=0;i<m_fastRenderItems.size();++i)
			if(m_fastRenderItems[i]->isDirty())
				return true;

		for(u32 i=0;i<m_children.size();++i)
			if(m_children[i]->isDirty())
				return true;

		//TODO 为什么不是return false;
		return m_bDirty;
	}
}