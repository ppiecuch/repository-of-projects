#ifndef _MUI_SKINLAYERITEM_H_
#define _MUI_SKINLAYERITEM_H_

#include "MUI_Prepare.h"
#include "MUI_LayerItem.h"

namespace mui{

	class SkinLayerItem : LayerItem{
	private:
		//VectorSubWidget mSubSkinChild;
		core::array<IBaseWidget*> m_skinWidgets;

		//ISubWidgetText* mText;
		//TODO

		//ISubWidgetRect* mMainSkin;
		IShapWidget* m_pShapWidget;

		//bool mSubSkinsVisible;
		bool m_bSkinVisible;
	public:
		SkinLayerItem();
	};
}
#endif