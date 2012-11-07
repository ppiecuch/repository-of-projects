#ifndef _YON_SCENE_SUNIT_H_
#define _YON_SCENE_SUNIT_H_

#include "IUnit.h"
#include "yonArray.h"

namespace yon{
namespace scene{

	template<size_t VT>
	struct SUnit : public virtual IUnit{
	protected:
		IShap* m_pShap;
		video::SMaterial m_material;
		video::ENUM_HARDWARDBUFFER_USAGE_TYPE m_vertexUsageType,m_indexUsageType;
	public:
		SUnit()
			:m_material(video::DEFAULT_MATERIAL),m_pShap(NULL),
			m_vertexUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE),m_indexUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE){
		}
		SUnit(const video::SMaterial& material)
			:m_material(material),m_pShap(NULL),
			m_vertexUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE),m_indexUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE){
		}

		virtual ~SUnit(){
			if(m_pShap)
				m_pShap->drop();
		}

		//virtual video::IMaterial* getMaterial(){
		//	return m_pMaterial;
		//}
		//virtual const video::IMaterial* getMaterial() const{
		//	return m_pMaterial;
		//}

		virtual video::SMaterial& getMaterial(){
			return m_material;
		}
		virtual const video::SMaterial& getMaterial() const{
			return m_material;
		}

		//virtual video::ENUM_HARDWARDBUFFER_USAGE_TYPE getHardwareBufferUsageType() const{
		//	return m_usageType;
		//}
		//virtual void setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE type){
		//	m_usageType=type;
		//}
		virtual video::ENUM_HARDWARDBUFFER_USAGE_TYPE getVertexHardwareBufferUsageType() const{
			return m_vertexUsageType;
		}
		virtual void setVertexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE type){
			m_vertexUsageType=type;
		}
		virtual video::ENUM_HARDWARDBUFFER_USAGE_TYPE getIndexHardwareBufferUsageType() const{
			return m_indexUsageType;
		}
		virtual void setIndexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE type){
			m_indexUsageType=type;
		}

		virtual IShap* getShap()const {
			return m_pShap;
		}
		virtual void setShap(IShap* shap){
			if(shap==NULL||shap==m_pShap)
				return;
			YON_DEBUG_BREAK_IF(shap->getVertexType()!=getVertexType());
			shap->grab();
			if(m_pShap)
				m_pShap->drop();
			m_pShap=shap;
		}

		virtual ENUM_VERTEX_TYPE getVertexType() const{
			return static_cast<const ENUM_VERTEX_TYPE>(VT);
		}
	};

	typedef SUnit<ENUM_VERTEX_TYPE_3V1T1C> Unit3D;
	typedef SUnit<ENUM_VERTEX_TYPE_2V1T1C> Unit2D;
	typedef SUnit<ENUM_VERTEX_TYPE_3V2T1C> Unit3D2T;
}
}
#endif