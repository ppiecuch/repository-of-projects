#ifndef _MUI_ILAYERNODE_H_
#define _MUI_ILAYERNODE_H_

#include "SMaterial.h"

using namespace yon;

namespace mui{

	class ILayer;
	class ILayerItem;
	class LayerItem;
	class RenderItem;
	class IRenderTarget;

	class ILayerNode{
	private:
		//只被LayerItem调用
		//virtual void detachLayerItem(ILayerItem* item) = 0;
		virtual void detach(ILayerItem* item) = 0;

		friend class LayerItem;
	public:
		virtual ~ILayerNode(){}

		//return the parent node or NULL
		virtual ILayerNode* getParent() const = 0;

		virtual ILayer* getLayer() const = 0;

		//add a child node and return it
		//virtual ILayerNode* createChildItemNode() = 0;
		virtual ILayerNode* addChild() = 0;

		//removes a child node
		//virtual void destroyChildItemNode(ILayerNode* _node) = 0;
		virtual void removeChild(ILayerNode* node) = 0;

		//raise a child node
		//virtual void upChildItemNode(ILayerNode* _node) = 0;
		virtual void raiseChild(ILayerNode* node) = 0;

		//virtual void attachLayerItem(ILayerItem* item) = 0;
		virtual void attach(ILayerItem* item) = 0;

		virtual u32 getChildrenCount() const = 0;

		//virtual RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _separate) = 0;
		virtual RenderItem* addToRenderItem(const video::SMaterial& material,bool standard) = 0;

		//virtual void outOfDate(RenderItem* _item) = 0;
		virtual void setDirty(RenderItem* item) = 0;

		virtual bool isDirty() const = 0;

		//returns the widget at the position
		//virtual ILayerItem* getLayerItemByPosition(s32 left, s32 top) const = 0;
		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const = 0;

		//virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;
		virtual void renderToTarget(IRenderTarget* target) = 0;

		virtual f32 getDepth() = 0;

	};
}
#endif