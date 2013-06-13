#ifndef _MUI_LAYERNODE_H_
#define _MUI_LAYERNODE_H_

#include "yonArray.h"

using namespace yon;

namespace mui{

	class RenderItem;

	class LayerNode : ILayerNode{

		ILayerNode* m_pParent;
		ILayer* m_pLayer;

		core::array<RenderItem*> m_stardardRenderItems;
		core::array<RenderItem*> m_fastRenderItems;

		core::array<ILayerItem*> m_layerItems;
		core::array<ILayerNode*> m_children;

		bool m_bDirty;
		f32 m_fDepth;
	protected:
		void updateStardardRenderItems();
	public:

		explicit LayerNode(ILayer* layer, ILayerNode* parent = NULL);
		virtual ~LayerNode();

		virtual ILayerNode* getParent() const;

		virtual ILayer* getLayer() const;

		virtual ILayerNode* addChild();

		virtual void removeChild(ILayerNode* node);

		virtual void raiseChild(ILayerNode* node);

		virtual u32 getChildrenCount() const;

		virtual RenderItem* addToRenderItem(const video::SMaterial& material,bool standard);

		virtual void setDirty(RenderItem* item);

		virtual bool isDirty() const;

		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const;

		virtual void renderToTarget(IRenderTarget* target);

		virtual f32 getDepth();
	};
}
#endif