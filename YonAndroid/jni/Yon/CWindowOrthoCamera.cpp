
#include "CWindowOrthoCamera.h"

namespace yon{
namespace scene{
namespace camera{

	CWindowOrthoCamera::CWindowOrthoCamera(const core::vector3df& pos,const core::vector3df& up,const core::vector3df& lookat):
		COrthoCamera(pos,up,lookat),m_originalPos(core::ORIGIN_POSITION3DF){
	}

	void CWindowOrthoCamera::recalculateViewMatrix(){
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].makeIdentity();
		m_matrixs[ENUM_FRUSTUM_TRANSFORM_VIEW].lookAt(m_position.x+m_originalPos.x,m_position.y+m_originalPos.y,m_position.z+m_originalPos.z,m_target.x+m_originalPos.x,m_target.y+m_originalPos.y,m_target.z+m_originalPos.z,m_up.x,m_up.y,m_up.z);
		m_bNeedUpload=true;
	}

	void CWindowOrthoCamera::onResize(const core::dimension2du& size){
		m_originalPos.x=size.w>>1;
		m_originalPos.y=size.h>>1;
		COrthoCamera::onResize(size);
	}
}
}
}