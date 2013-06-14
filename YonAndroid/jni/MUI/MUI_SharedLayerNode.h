#ifndef _MUI_SHAREDLAYERNODE_H_
#define _MUI_SHAREDLAYERNODE_H_

namespace mui{

	class SharedLayerNode : LayerNode{
	private:
		u32 m_uRegisterCount;
	public:
		explicit SharedLayerNode(ILayer* layer, ILayerNode* parent = NULL);
		virtual ~SharedLayerNode();

		//void addUsing();
		void register();

		//void removeUsing();
		void unregister();

		//size_t countUsing() const;
		u32 getRegisterCount() const;
	};
}
#endif