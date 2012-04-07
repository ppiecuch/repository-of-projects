#ifndef _YON_SCENE_SUNIT_H_
#define _YON_SCENE_SUNIT_H_

#include "IUnit.h"
#include "yonArray.h"
#include "IMaterial.h"

namespace yon{
namespace scene{

	template<size_t M>
	struct SUnit : public IUnit{
	private:
		IShap* m_pShap;

		video::IMaterial* m_pMaterial;
	public:
		SUnit()
			:m_pMaterial(video::DEFAULT_MATERIAL),m_pShap(NULL){
				m_pMaterial->grab();
		}

		virtual ~SUnit(){
			m_pMaterial->drop();
			if(m_pShap)
				m_pShap->drop();
		}

		virtual video::IMaterial* getMaterial(){
			return m_pMaterial;
		}
		virtual const video::IMaterial* getMaterial() const{
			return m_pMaterial;
		}

		virtual const IShap* getShap()const {
			return m_pShap;
		}
		virtual void setShap(IShap* shap){
			YON_DEBUG_BREAK_IF(shap->getDimenMode()!=getDimenMode());
			shap->grab();
			m_pShap=shap;
		}

		virtual ENUM_DIMEN_MODE getDimenMode() const{
			return static_cast<const ENUM_DIMEN_MODE>(M);;
		

	};
}
}
#endif