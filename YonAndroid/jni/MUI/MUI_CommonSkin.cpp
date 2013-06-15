#include "MUI_CommonSkin.h"

namespace mui{

	CommonSkin::CommonSkin()
		:m_color(video::COLOR_WHITE),
		m_pNode(NULL),m_pRenderItem(NULL)
	{
	}
	CommonSkin::~CommonSkin(){}

	void CommonSkin::updateView(){}
	void CommonSkin::repairView(){}

	void CommonSkin::setAlign(const dimension2di& old){}

	void CommonSkin::setUVRect(const core::rectf& rect){}
	void CommonSkin::setColor(const video::SColor& value){
		m_color=value;
		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

	//TODO 是否可与setColor合并
	void CommonSkin::setAlpha(f32 alpha){
		m_color.setAlpha(video::SColor::floatToU8(alpha));
		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

	void CommonSkin::setVisible(bool on){
		if(m_bVisible==on)
			return;
		m_bVisible=on;
		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

	void CommonSkin::createDrawItem(const video::SMaterial& material, ILayerNode* node){
		YON_DEBUG_BREAK_IF(m_pRenderItem!=NULL);

		m_pNode = node;
		m_pRenderItem = m_pNode->addToRenderItem(material, true);
		m_pRenderItem->addDrawItem(this,6);
	}
	void CommonSkin::destroyDrawItem(){
		YON_DEBUG_BREAK_IF(m_pRenderItem==NULL);

		m_pNode=NULL;
		m_pRenderItem->removeDrawItem(this);
		m_pRenderItem=NULL;
	}

	void CommonSkin::preRender(){}
}