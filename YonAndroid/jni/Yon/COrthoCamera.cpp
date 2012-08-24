#include "COrthoCamera.h"
#include "ISceneManager.h"
#include "ILogger.h"

namespace yon{
namespace scene{
namespace camera{
	COrthoCamera::COrthoCamera(IModel* parent,const core::vector3df& pos,const core::vector3df& up,const core::vector3df& lookat):
		IOrthoCamera(parent,pos,up,lookat){
			recalculateProjectionMatrix();
			recalculateViewMatrix();
	}
	void COrthoCamera::onRegisterForRender(){
		//if(m_pSceneManager->getActiveCamera()==this)
		if(m_pSceneManager->getViewingCamera()==this)
			m_pSceneManager->registerForRender(this,ENUM_SCENE_PASS_CAMERA);
		IOrthoCamera::onRegisterForRender();
	}
	void COrthoCamera::recalculateProjectionMatrix(){
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION].makeIdentity();
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION].ortho(m_fLeft,m_fRight,m_fBottom,m_fTop,m_fNear,m_fFar);
		m_bNeedUpload=true;
	}
	//TODO³éÏóµ½ICameraÖÐ
	void COrthoCamera::recalculateViewMatrix(){
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].makeIdentity();
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].lookAt(m_position.x,m_position.y,m_position.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
		m_bNeedUpload=true;
	}

	void COrthoCamera::render(video::IVideoDriver* driver){
		if(m_bNeedUpload==false)
			return;
		driver->setTransform(video::ENUM_TRANSFORM_VIEW,m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW]);
		driver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION]);
		m_bNeedUpload=false;
	}

	void COrthoCamera::onResize(const core::dimension2du& size){
		m_fRight=(f32)(size.w>>1);
		m_fLeft=m_fRight-size.w;
		m_fTop=(f32)(size.h>>1);
		m_fBottom=m_fTop-size.h;
		recalculateViewMatrix();
		recalculateProjectionMatrix();
		Logger->debug("COrthoCamera::onResize->recalculateProjectionMatrix\n");
	}

	void COrthoCamera::setNear(f32 nearValue){
		IOrthoCamera::setNear(nearValue);
		recalculateProjectionMatrix();
	}

	void COrthoCamera::setFar(f32 farValue){
		IOrthoCamera::setFar(farValue);
		recalculateProjectionMatrix();
	}

	void COrthoCamera::setPosition(const core::vector3df& pos,bool symport){
		IOrthoCamera::setPosition(pos,symport);
		recalculateViewMatrix();
	}
	void COrthoCamera::setRotation(const core::vector3df& rot){
		IOrthoCamera::setRotation(rot);
		recalculateViewMatrix();
	}
}
}
}