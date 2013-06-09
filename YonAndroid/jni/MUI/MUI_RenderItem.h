#ifndef _MUI_RENDERITEM_H_
#define _MUI_RENDERITEM_H_

#include "MUI_prerequest.h"

namespace mui{

class MUI_API RenderItem{
public:
	RenderItem();
	virtual ~RenderItem();

	//void renderToTarget(IRenderTarget* _target, bool _update);
	void renderToTarget(IRenderTarget* _target);
	IRenderTarget* getRenderTarget();

	//void outOfDate();
	//bool isOutOfDate() const;
	void setDirty();
	bool isDirty() const;
};

}
#endif