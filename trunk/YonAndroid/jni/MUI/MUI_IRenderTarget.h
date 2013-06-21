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

	//ֻ��RenderManager��RTTTexture���õ������Ҷ���1,������ʱ�رմ˱���
	//float maximumDepth;
	//ֻ��RenderManager���õ�:setViewSize(...)		//TODO �Ƿ�Ҳ���ô˻��ƣ��Ͼ�ֱ��ʹ�ö��������ܸ��ã���ʱ��ԭ���ƽ��У�
	//mInfo.pixScaleX = 1.0f / float(mViewSize.width);
	//mInfo.pixScaleY = 1.0f / float(mViewSize.height);
	//f32 pixScaleX;
	//f32 pixScaleY;
	//��׷�٣��̳���IRenderTarget��ֻ��RenderManager��RTTTexture�������ߵ�hOffset,vOffset����0��������ʱ�ر���Щ����
	//����leftOffset,topOffsetҲ��û���õ���ֻ��RenderTargetInfo�Ľӿ�setOffset���õ�
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