#ifndef _MUI_IBASEWIDGET_H_
#define _MUI_IBASEWIDGET_H_

#include "MUI_ICroppedRetangle.h"

namespace mui{

	class ILayerNode;

	class IBaseWidget : ICroppedRetangle{
	public:

		//virtual void doRender() = 0;
		virtual void preRender() = 0;

		//virtual void createDrawItem(ITexture* _texture, ILayerNode* _node) = 0;
		virtual void createDrawItem(const video::SMaterial& material, ILayerNode* node) = 0;
		virtual void destroyDrawItem() = 0;
	};

}
#endif