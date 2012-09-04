#include "MyGUIRTTexture.h"

namespace MyGUI{

	MyGUIRTTexture::MyGUIRTTexture(unsigned int _texture){
		
		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);
	}
	MyGUIRTTexture::~MyGUIRTTexture(){

	}

	void MyGUIRTTexture::begin(){

	}
	void MyGUIRTTexture::end(){

	}

	void MyGUIRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count){

	}

}