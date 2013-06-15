#ifndef _MUI_IBASEWIDGET_H_
#define _MUI_IBASEWIDGET_H_

#include "MUI_ICroppedRetangle.h"

namespace mui{

	class ILayerNode;

	class IBaseWidget : ICroppedRetangle{
	protected:
		Align m_align;
		bool m_bVisible;
	public:
		ISubWidget() : m_bVisible(true){}
		virtual ~ISubWidget(){}

		//virtual void doRender() = 0;
		virtual void preRender() = 0;

		//virtual void createDrawItem(ITexture* _texture, ILayerNode* _node) = 0;
		virtual void createDrawItem(const video::SMaterial& material, ILayerNode* node) = 0;
		virtual void destroyDrawItem() = 0;

		virtual void setAlpha(f32 alpha) {}

		//virtual void setAlign(Align value)
		virtual void setAlign(const Align& value)
		{
			m_align = value;
		}

		virtual void setVisible(bool on)
		{
			m_bVisible = on;
		}

	};

}
#endif