#include "CSceneManager.h"
#include "CGeometryFactory.h"
#include "CModel.h"

#include "ILogger.h"

namespace yon{
	namespace scene{

		CSceneManager::CSceneManager():
			m_geometryFactory(new CGeometryFactory()),
			IModel(NULL){
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CSceneManager");
		}

		CSceneManager::~CSceneManager(){
			m_geometryFactory->drop();
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Release CSceneManager");
		}

		IModel* CSceneManager::addModel(IEntity* entity){
			CModel* model=new CModel(this,entity);
			addChild(model);
			model->drop();

			Logger->debug("CSceneManager::addModel size:%d\n",m_children.size());
			return model;
		}

		void CSceneManager::render(video::IVideoDriver* driver){
			core::list<IModel*>::Iterator it = m_children.begin();
			for (; it != m_children.end(); ++it)
			{
				(*it)->render(driver);
			}
		}

		const IGeometryFactory* CSceneManager::getGeometryFactory() const{
			return m_geometryFactory;
		}
	}
}