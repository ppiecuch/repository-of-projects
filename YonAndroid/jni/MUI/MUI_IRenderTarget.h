#ifndef _MUI_IRENDERTARGET_H_
#define _MUI_IRENDERTARGET_H_

#include "SMaterial.h"
#include "IShap.h"

using namespace yon;

namespace mui{

class MUI_API IRenderTarget{
public:
	virtual ~IRenderTarget() {}

	virtual void begin() = 0;
	virtual void doRender(const video::SMaterial& material,scene::IShap* shap);
	virtual void end() = 0;
};
}
#endif