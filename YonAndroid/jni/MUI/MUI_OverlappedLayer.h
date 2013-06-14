#ifndef _MUI_OVERLAPPEDLAYER_H_
#define _MUI_OVERLAPPEDLAYER_H_

namespace mui{

	//OverlappedLayer 可以覆盖的窗口。每个根窗口一个渲染批次。
	class OverlappedLayer : ILayer{
	protected:
		//SOLVED 可能是pickable==>果然是
		//bool m_bPicked;
		bool m_bEventReceiable;
		bool m_bDirty;
		core::dimension2di m_viewSize;

		core::array<ILayerNode*> m_childNodes;
	public:
		OverlappedLayer();
		virtual ~OverlappedLayer();

		virtual ILayerNode* addChildNode() = 0;
		virtual void removeChildNode(ILayerNode* node) = 0;

		virtual void raiseChildNode(ILayerNode* node) = 0;

		virtual u32 getChildNodeCount() const = 0;

		virtual ILayerNode* getChildNodeAt(u32 index) const = 0;

		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const = 0;

		virtual const core::dimension2di& getSize() const = 0;

		virtual void renderToTarget(IRenderTarget* _target) = 0;

		virtual void onResize(const core::dimension2di& size) = 0;

		//bool isOutOfDate() const;
		bool isDirty() const;
	};
}
#endif