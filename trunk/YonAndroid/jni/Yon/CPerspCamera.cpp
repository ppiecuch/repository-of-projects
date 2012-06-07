#include "CPerspCamera.h"
#include "ILogger.h"

namespace yon{
namespace scene{
namespace camera{
CPerspCamera::CPerspCamera(const core::vector3df& pos,const core::vector3df& up,const core::vector3df& lookat):
	IPerspCamera(pos,up,lookat){
	recalculateProjectionMatrix();
	recalculateViewMatrix();
}
void CPerspCamera::recalculateProjectionMatrix(){
	m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION].makeIdentity();
	m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION].perspective(m_fFovy,m_fAspectRatio,m_fNear,m_fFar);
	m_bNeedUpload=true;
}
//TODO³éÏóµ½ICameraÖÐ
void CPerspCamera::recalculateViewMatrix(){
	m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].makeIdentity();
	m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].lookAt(m_position.x,m_position.y,m_position.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
	Logger->debug("CPerspCamera.pos:%.2f,%.2f,%.2f\n",m_position.x,m_position.y,m_position.z);
	m_bNeedUpload=true;
}

void CPerspCamera::render(video::IVideoDriver* driver){
	if(m_bNeedUpload==false)
		return;
	driver->setTransform(video::ENUM_TRANSFORM_VIEW,m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW]);
	driver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_matrixs[ENUM_FRUSTUM_TRANSFORM_PROJECTION]);
	m_bNeedUpload=false;
}

void CPerspCamera::onResize(const core::dimension2du& size){
	m_fAspectRatio=(f32)size.w/size.h;
	recalculateViewMatrix();
	recalculateProjectionMatrix();
	Logger->debug("CPerspCamera::onResize->recalculateProjectionMatrix:%.2f\n",m_fAspectRatio);
}
void CPerspCamera::setAspectRatio(f32 aspect){
	IPerspCamera::setAspectRatio(aspect);
	recalculateProjectionMatrix();
}
void CPerspCamera::setFovy(f32 fovy){
	IPerspCamera::setFovy(fovy);
	recalculateProjectionMatrix();
}
void CPerspCamera::setNear(f32 near){
	IPerspCamera::setNear(near);
	recalculateProjectionMatrix();
}

void CPerspCamera::setFar(f32 far){
	IPerspCamera::setFar(far);
	recalculateProjectionMatrix();
}

void CPerspCamera::setPosition(const core::vector3df& pos){
	IPerspCamera::setPosition(pos);
	recalculateViewMatrix();
}
void CPerspCamera::setRotation(const core::vector3df& rot){
	IPerspCamera::setRotation(rot);
	recalculateViewMatrix();
}
}//camera
}//scene
}//yon