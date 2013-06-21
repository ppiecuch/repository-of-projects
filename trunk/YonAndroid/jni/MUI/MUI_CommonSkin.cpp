#include "MUI_CommonSkin.h"
#include "MUI_Fill.h"

namespace mui{

	CommonSkin::CommonSkin()
		:m_color(video::COLOR_WHITE),
		m_pNode(NULL),m_pRenderItem(NULL)
	{
	}
	CommonSkin::~CommonSkin(){}

	void CommonSkin::updateCroppedMargin()
	{
		/*
		float UV_lft = mMargin.left / (float)mCoord.width;
		float UV_top = mMargin.top / (float)mCoord.height;
		float UV_rgt = (mCoord.width - mMargin.right) / (float)mCoord.width;
		float UV_btm = (mCoord.height - mMargin.bottom) / (float)mCoord.height;

		float UV_sizeX = mRectTexture.right - mRectTexture.left;
		float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

		float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
		float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
		float UV_rgt_total = mRectTexture.right - (1 - UV_rgt) * UV_sizeX;
		float UV_btm_total = mRectTexture.bottom - (1 - UV_btm) * UV_sizeY;

		mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
		*/

		f32 uvLft=m_margin.Left/(f32)m_coordinate.getWidth();
		f32 uvTop=m_margin.Top/(f32)m_coordinate.getHeight();
		f32 uvRgt=(m_coordinate.getWidth()-m_margin.Right)/(f32)m_coordinate.getWidth();
		f32 uvBtm=(m_coordinate.getHeight()-m_margin.Bottom)/(f32)m_coordinate.getHeight();

		f32 uvSizeX=m_croppedMargin.Right-m_croppedMargin.Left;
		f32 uvSizeY=m_croppedMargin.Bottom-m_croppedMargin.Top;

		f32 uvLftTotal=m_croppedMargin.Left+uvLft*uvSizeX;
		f32 uvTopTotal=m_croppedMargin.Top+uvTop*uvSizeY;
		f32 uvRgtTotal=m_croppedMargin.Right-(1-uvRgt)*uvSizeX;
		f32 uvBtmTotal=m_croppedMargin.Bottom-(1-uvBtm)*uvSizeY;

		m_croppedMargin.set(uvLftTotal,uvTopTotal,uvRgtTotal,uvBtmTotal);
	}

	void CommonSkin::setUVRect(const core::marginf& rect)
	{
		if(m_croppedMargin==rect)
			return;
		m_croppedMargin=rect;

		if(m_bMargin)
		{
			updateCroppedMargin();
		}
		else
		{
			m_croppedMargin=m_unknownMargin;
		}

		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

	void CommonSkin::updateView(){

		//TODO 待理解
		bool result=updateMargin();

		m_bEmptyView=getViewWidth()<=0||getViewHeight()<=0;

		m_croppedCoordinate.topLeft.set(m_coordinate.topLeft.x+m_margin.Left,m_coordinate.topLeft.y+m_margin.Top);

		if(result)
		{
			if(isFullCropped())
			{
				m_bMargin=result;
				if(m_pNode)
					m_pNode->setDirty(m_pRenderItem);
				return;
			}
		}

		if(m_bMargin||result)
		{
			m_croppedCoordinate.bottomRight.x=m_croppedCoordinate.topLeft.x+getViewWidth();
			m_croppedCoordinate.bottomRight.y=m_croppedCoordinate.topLeft.y+getViewHeight();

			if(m_croppedCoordinate.getWidth()>0||m_croppedCoordinate.getHeight()>0)
			{
				updateCroppedMargin();
			}
		}

		if(m_bMargin&&!result)
		{
			//mCurrentTexture = mRectTexture;
			m_croppedMargin=m_unknownMargin;
		}
		//mIsMargin = margin;
		m_bMargin=result;

		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}
	void CommonSkin::repairView(){
		if(m_pNode)
			m_pNode->setDirty(m_pRenderItem);
	}

	void CommonSkin::setAlign(const dimension2di& old){
		YON_DEBUG_BREAK_IF(m_pParent->getWidth()<m_coordinate.getWidth()||m_pParent->getHeight()<m_coordinate.getHeight());
		//SOLVED 待理解
		bool needUpdate=false;

		if(m_align.isHStretch())
		{
			//mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _oldsize.width);
			//old.w应该是m_coordinate原来的width，这样以下算式就相当于
			//m_coordinate.bottomRight.x=m_coordinate.bottomRight.x+(m_pParent->getWidth() - m_coordinate.getWidth())
			//							=m_coordinate.topLeft.x+m_pParent->getWidth()
			m_coordinate.bottomRight.x=m_coordinate.bottomRight.x+(m_pParent->getWidth() - old.w);
			needUpdate=true;
			m_bMargin=true;
		}
		else if(m_align.isRight())
		{
			//mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _oldsize.width);
			//m_pParent->getWidth()-old.w相当于父控件-本控件的空隙长度
			//m_coordinate.topLeft.x+空隙长度即为向右对齐
			m_coordinate.topLeft.x=m_coordinate.topLeft.x+m_pParent->getWidth()-old.w;
			needUpdate=true;
		}
		else if(m_align.isHCenter())
		{
			//mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			//显然是居中算式
			m_coordinate.topLeft.x=(m_pParent->getWidth()-m_coordinate.getWidth())/2;
			needUpdate=true;
		}

		if(m_align.isVStretch())
		{
			//mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _oldsize.height);
			m_coordinate.bottomRight.y=m_coordinate.bottomRight.y+m_pParent->getHeight()-old.h;
			needUpdate=true;
			m_bMargin=true;
		}
		else if(m_align.isBottom())
		{
			//mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _oldsize.height);
			m_coordinate.topLeft.y=m_coordinate.topLeft.y+m_pParent->getHeight()-old.h;
			needUpdate=true;
		}
		else if(m_align.isVCenter())
		{
			//mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			m_coordinate.topLeft.y=(m_pParent->getHeight()-m_coordinate.getHeight())/2;
			needUpdate=true;
		}

		if(needUpdate)
		{
			m_croppedCoordinate=m_coordinate;
			updateView();
		}
	}

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

	//virtual void doRender();
	void CommonSkin::preRender(){
		if(!m_bVisible||m_bEmptyView)
			return;

		//m_pRenderItem->get
		scene::SDynamicShap3D& shap=m_pRenderItem->getShap();
		core::array<SVertex>& vertices=shap.getVertexArray();
		core::array<u16>& indices=shap.getIndexArray();
		IRenderTarget* rt=m_pRenderItem->getRenderTarget();

		f32 depth = m_pNode->getDepth();

		//float vertex_left = ((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) + info.hOffset) * 2) - 1;
		//float vertex_right = vertex_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
		//float vertex_top = -(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) + info.vOffset) * 2) - 1);
		//float vertex_bottom = vertex_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

		f32 left=rt->getScaleToNormal().x*(f32)(m_croppedCoordinate.topLeft.x+m_pParent->getAbsoluteLeft())*2-1;
		f32 right=left+rt->getScaleToNormal().x*(f32)m_croppedCoordinate.getWidth()*2;
		f32 top=-((rt->getScaleToNormal().y*(f32)m_croppedCoordinate.topLeft.y+m_pParent->getAbsoluteTop())*2-1);
		f32 bottom=top-rt->getScaleToNormal().y*m_croppedCoordinate.getHeight()*2;

		vertices.set_used(4);
		indices.set_used(6);

	}
}