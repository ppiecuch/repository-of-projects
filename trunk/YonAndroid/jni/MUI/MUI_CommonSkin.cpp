#include "MUI_CommonSkin.h"

namespace mui{

	CommonSkin::CommonSkin()
		:m_color(video::COLOR_WHITE),
		m_pNode(NULL),m_pRenderItem(NULL)
	{
	}
	CommonSkin::~CommonSkin(){}

	void CommonSkin::updateView(){

		bool bMargin=updateMargin();

		m_bEmptyView=getViewWidth()<=0||getViewHeight()<=0;

		m_croppedCoordinate.topLeft.set(m_coordinate.topLeft.x+m_margin.Left,m_coordinate.topLeft.y+m_margin.Top);

		if(bMargin)
		{
			if(isFullCropped())
			{
				m_bMargin=bMargin;
				if(m_pNode)
					m_pNode->setDirty(m_pRenderItem);
				return;
			}
		}

		if(m_bMargin||bMargin)
		{
			m_croppedCoordinate.bottomRight.x=m_croppedCoordinate.topLeft.x+getViewWidth();
			m_croppedCoordinate.bottomRight.y=m_croppedCoordinate.topLeft.y+getViewHeight();

			if(m_croppedCoordinate.getWidth()>0||m_croppedCoordinate.getHeight()>0)
			{

			}
		}
	}
	void CommonSkin::repairView(){
		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

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