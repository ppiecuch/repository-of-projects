#ifndef _MUI_ILAYERNODE_H_
#define _MUI_ILAYERNODE_H_

namespace mui{

	class ILayer;
	class ILayerItem;
	class RenderItem;
	class IRenderTarget;

	class ILayerNode{
	public:
		virtual ~ILayerNode(){}

		//return the parent node or NULL
		virtual ILayerNode* getParent() const = 0;

		virtual ILayer* getLayer() const = 0;

		//create a child node
		virtual ILayerNode* createChild() = 0;

		//removes a child node
		virtual void removeChild(ILayerNode* node) = 0;

		//raise a child node
		virtual void raiseChild(ILayerNode* node) = 0;

		//virtual RenderItem* addToRenderItem(ITexture* _texture, bool _firstQueue, bool _separate) = 0;
		virtual RenderItem* addToRenderItem(const video::SMaterial& material) = 0;

		//virtual void outOfDate(RenderItem* _item) = 0;
		virtual void setDirty(RenderItem* item) = 0;

		//returns the widget at the position
		//virtual ILayerItem* getLayerItemByPosition(s32 left, s32 top) const = 0;
		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const = 0;

		//virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;
		virtual void renderToTarget(IRenderTarget* target) = 0;

		virtual f32 getDepth() = 0;

	};
}
#endif