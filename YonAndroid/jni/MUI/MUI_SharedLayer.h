#ifndef _MUI_SHAREDLAYER_H_
#define _MUI_SHAREDLAYER_H_

namespace mui{
	
	//SharedLayer 不能覆盖。可以覆盖的窗口在这个图层里面drawn可能尝试错误。整个SharedLayer类型的图层在一个渲染批次里面。
	class SharedLayer : ILayer{
	protected:
		//SOLVED 可能是pickable==>果然是
		//bool m_bPicked;
		bool m_bEventReceiable;
		bool m_bDirty;
		core::dimension2di m_viewSize;

		SharedLayerNode* m_pNode;
	public:
		SharedLayer();
		virtual ~SharedLayer();

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