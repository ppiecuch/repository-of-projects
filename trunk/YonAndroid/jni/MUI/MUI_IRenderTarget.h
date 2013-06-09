#ifndef _MUI_IRENDERTARGET_H_
#define _MUI_IRENDERTARGET_H_

namespace mui{

class MUI_API IRenderTarget{
public:
	virtual ~IRenderTarget() {}

	virtual void begin() = 0;
	virtual void end() = 0;
};
}
#endif