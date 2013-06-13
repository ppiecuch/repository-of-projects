#include "MUI_RenderItem.h"

namespace mui{

	RenderItem::RenderItem():
			m_bDirty(true),
			m_pRenderTarget(NULL),
			m_uRequestVertexCount(0),
			m_bCompression(false)
	{
	}

	RenderItem::~RenderItem(){
	}

	void RenderItem::renderToTarget(IRenderTarget* target){
		m_pRenderTarget=target;

		if(m_bDirty)
		{
			for(u32 i=0;i<m_drawItems.size();++i)
				m_drawItems[i].Widget->preRender();
			m_bDirty=false;
		}
		m_pRenderTarget->doRender(m_material,&m_shap);
	}

	IRenderTarget* RenderItem::getRenderTarget(){
		return m_pRenderTarget;
	}

	void RenderItem::setMaterial(const video::SMaterial& material){
		m_material=material;
	}

	const video::SMaterial& RenderItem::getMaterial(){
		return m_material;
	}

	void RenderItem::setDirty(){
		m_bDirty=true;
	}

	bool RenderItem::isDirty() const{
		return m_bDirty;
	}

	bool RenderItem::isCompression(){
		//TODO:疑问，不懂？
		bool result = m_bCompression;
		m_bCompression = false;
		return result;
	}

	u32 RenderItem::getRequestVertexCount() const{
		return m_uRequestVertexCount;
	}

	u32 RenderItem::getVertexCount() const{
		return m_shap.getVertexCount();
	}

	void RenderItem::addDrawItem(IBaseWidget* widget, size_t count){
		SDrawItem(widget,count) item;
		m_drawItems.push_back(item);
		m_bDirty=true;
	}

	void RenderItem::removeDrawItem(IBaseWidget* widget){
		YON_DEBUG_BREAK_IF(widget==NULL);
		for(u32 i=0;i<m_drawItems.size();++i)
		{
			if(m_drawItems[i].Widget!=widget)
				continue;
			m_drawItems.erase(i);
			m_bDirty=true;

			//TODO:疑问，不懂？
			if (m_drawItems.empty())
			{
				mCompression = true;
			}

			return;
		}
	}

	void RenderItem::reallockDrawItem(IBaseWidget* widget, size_t count){
		//TODO
	}
}