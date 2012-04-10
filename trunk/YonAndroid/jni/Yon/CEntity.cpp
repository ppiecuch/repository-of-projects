#include "CEntity.h"
#include "SUnit.h"

#include "ILogger.h"

namespace yon{
namespace scene{

	void CEntity::add(IUnit* unit){
		if(unit!=NULL){
			//TODO ������yonArray.reallocate�е�delete[] temp����ͻ?
			unit->grab();
			m_units.push(unit);
		}
	}

	void CEntity::add(video::ITexture* texture,video::ENUM_MATERIAL_TYPE materialType,IShap* shap){
		//�����texture��Ӧ��unitֱ��ʹ��֮
		IUnit* unit=getUnit(texture,materialType);
		if(unit==NULL){
			switch(shap->getVertexType())
			{
			case ENUM_VERTEX_TYPE_2V1T1C:
				unit=new Unit2D();
				break;
			case ENUM_VERTEX_TYPE_3V2T1C:
				unit=new Unit3D2T();
				break;
			default:
				unit=new Unit3D();
			}
			unit->getMaterial()->setMaterialType(materialType);
			unit->getMaterial()->setTexture(0,texture);
			unit->setShap(shap);
			add(unit);
			unit->drop();
		}else{
			unit->getShap()->append(shap);
		}

	}

	IUnit* CEntity::getUnit(video::ITexture* texture,video::ENUM_MATERIAL_TYPE materialType){
		//TODO�Ż�
		for(u32 i=0;i<m_units.size();++i){
			if(m_units[i]->getMaterial()->getTexture(0)==texture){
				Logger->debug("CEntity::getUnit: find one!\n");
				return m_units[i];
			}
		}
		Logger->debug("CEntity::getUnit: find none!\n");
		return NULL;
	}

}
}