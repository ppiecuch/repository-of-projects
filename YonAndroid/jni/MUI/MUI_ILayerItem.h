#ifndef _MUI_ILAYERITEM_H_
#define _MUI_ILAYERITEM_H_

#include "MUI_Prepare.h"

namespace mui{

	class ILayerItem{
	private:
		//只被LayerNode调用
		//virtual void attachItemToNode(ILayer* layer, ILayerNode* node) = 0;
		virtual void attach(ILayer* layer, ILayerNode* node) = 0;

		friend class LayerNode;
	public:
		ILayerItem();
		virtual ~ILayerItem(){}

		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const = 0;
		virtual const core::recti& getLayerItemCoordinate() const = 0;

		//virtual void resizeLayerItemView(const IntSize& _oldView, const IntSize& _newView) = 0;

		//virtual void detachFromLayer() = 0;
		virtual void detach() = 0;

		virtual void raiseLayerItem() = 0;
	};
}
#endif