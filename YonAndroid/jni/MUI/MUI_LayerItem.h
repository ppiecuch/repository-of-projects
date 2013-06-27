#ifndef _MUI_LAYERITEM_H_
#define _MUI_LAYERITEM_H_

#include "yonArray.h"

namespace mui{

	class IBaseWidget;

	class LayerItem : ILayerItem{
		ILayer* m_pLayer;
		ILayerNode* m_pLayerNode;
		//TODO 废弃
		ILayerNode* m_pSavedLayerNode;

		//VectorSubWidget mDrawItems;
		core::array<IBaseWidget*> m_widgets;
		core::array<LayerItem*> m_children;
		core::array<LayerItem*> m_independents;

		//废弃了setMaterial，将此成员变量从private改成protected
	protected:
		video::SMaterial m_material;
	public:
		LayerItem();
		virtual ~LayerItem();

		ILayer* getLayer() const;
		ILayerNode* getLayerNode() const;

		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const;
		virtual const core::recti& getLayerItemCoordinate() const;

		virtual void attach(ILayer* layer, ILayerNode* node);
		virtual void detach();
		virtual void raiseLayerItem();

		//经查证，此API只被子类SkinLayerItem使用，废弃，将m_material直接开放给子类使用
		//void setRenderItemTexture(ITexture* _texture);
		//void setMaterial(const video::SMaterial& material);

		void addChildItem(LayerItem* item);
		void removeChildItem(LayerItem* item);

		void addChildNode(LayerItem* item);
		void removeChildNode(LayerItem* item);

		//void addRenderItem(ISubWidget* item);
		void addWidget(IBaseWidget* widget);
		//void removeAllRenderItems();
		void removeAllWidgets();

		void saveLayerItem();
		void restoreLayerItem();

	protected:
		void attachToLayerItemNode(ILayerNode* node, bool deep);
		void detachFromLayerItemNode(bool deep);
	};
}
#endif