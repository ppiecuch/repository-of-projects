#ifndef _MUI_IRENDERTARGET_H_
#define _MUI_IRENDERTARGET_H_

#include "SMaterial.h"
#include "IShap.h"

using namespace yon;

namespace mui{

class MUI_API IRenderTarget{
protected:
	core::vector2df m_scaleToNormal;
	f32 m_fAspectRatio;
	//TODO
	//core::vector2df m_offset;

	//只在RenderManager和RTTTexture中用到，而且都是1,所以暂时关闭此变量
	//float maximumDepth;
	//只在RenderManager中用到:setViewSize(...)		//TODO 是否也采用此机制，毕竟直接使用定点数性能更好（暂时按原机制进行）
	//mInfo.pixScaleX = 1.0f / float(mViewSize.width);
	//mInfo.pixScaleY = 1.0f / float(mViewSize.height);
	//f32 pixScaleX;
	//f32 pixScaleY;
	//经追踪，继承自IRenderTarget的只有RenderManager跟RTTTexture，但两者的hOffset,vOffset都是0，所以暂时关闭这些变量
	//另外leftOffset,topOffset也是没有用到，只在RenderTargetInfo的接口setOffset中用到
	//f32 hOffset;
	//f32 vOffset;
	//f32 aspectCoef;
	//s32 leftOffset;
	//s32 topOffset;
public:
	IRenderTarget():m_scaleFactor(core::IDENTITY_VECTOR2DF),m_fAspectRatio(1){}
	virtual ~IRenderTarget() {}

	virtual void begin() = 0;
	virtual void doRender(const video::SMaterial& material,scene::IShap* shap);
	virtual void end() = 0;

	//from RenderTargetInfo
	const core::vector2df& getScaleToNormal() const{
		return m_scaleToNormal;
	}

	f32 getAspectRatio() const{
		return m_fAspectRatio;
	}


};
}
#endif