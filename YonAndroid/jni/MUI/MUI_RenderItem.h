#ifndef _MUI_RENDERITEM_H_
#define _MUI_RENDERITEM_H_

#include "MUI_prerequest.h"
#include "yonArray.h"
#include "SDynamicShap.h"

using namespace yon;

namespace mui{

class MUI_API RenderItem{

	struct SDrawItem{
		SDrawItem(IBaseWidget* widget,size_t count): Widget(widget),Count(count){}
		IBaseWidget* Widget;
		u32 Count;
	};

	bool m_bDirty;
	bool m_bCompression;
	u32 m_uRequestVertexCount;
	video::SMaterial m_material;
	core::array<SDrawItem> m_drawItems;
	IRenderTarget* m_pRenderTarget;
	scene::SDynamicShap2D m_shap;
public:
	RenderItem();
	virtual ~RenderItem();

	//void renderToTarget(IRenderTarget* _target, bool _update);
	void renderToTarget(IRenderTarget* target);
	IRenderTarget* getRenderTarget();

	//void setTexture(ITexture* _value);
	//ITexture* getTexture();
	void setMaterial(const video::SMaterial& material);
	const video::SMaterial& getMaterial();

	//void outOfDate();
	//bool isOutOfDate() const;
	void setDirty();
	bool isDirty() const;

	bool isCompression();

	u32 getRequestVertexCount() const;
	u32 getVertexCount() const;

	void addDrawItem(IBaseWidget* widget, size_t count);
	void removeDrawItem(IBaseWidget* widget);
	void reallockDrawItem(IBaseWidget* widget, size_t count);
};

}
#endif