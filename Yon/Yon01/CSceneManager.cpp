#include "CSceneManager.h"
#include "CGeometryFactory.h"

#include "ILogger.h"

namespace yon{
	namespace scene{

		CSceneManager::CSceneManager():
			m_geometryFactory(new CGeometryFactory()){
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CSceneManager");
		}

		CSceneManager::~CSceneManager(){
			m_geometryFactory->drop();
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Release CSceneManager");
		}

		IModel* addModel(IEntity* entity){}

		void CSceneManager::render(){
		}

		const IGeometryFactory* CSceneManager::getGeometryFactory() const{
			return m_geometryFactory;
		}
	}
}