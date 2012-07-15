#ifndef _YON_SCENE_CSCENEMANAGER_H_
#define _YON_SCENE_CSCENEMANAGER_H_

#include "ISceneManager.h"
#include "IModel.h"
#include "IGeometryFactory.h"
#include "yonArray.h"
#include "ITimer.h"

namespace yon{
	namespace scene{

		class IVideoDriver;
		class CSceneManager : public ISceneManager, public IModel{
		//class CSceneManager : public ISceneManager{
		protected:

			struct DefaultModelEntry
			{
				DefaultModelEntry(IModel* m) : m_pModel(m), m_pTexturePointer(0)
				{
					if (m->getMaterialCount())
						m_pTexturePointer = (m->getMaterial(0)->getTexture(0));
				}

				bool operator < (const DefaultModelEntry& other) const
				{
					return (m_pTexturePointer < other.m_pTexturePointer);
				}

				IModel* m_pModel;
			private:
				void* m_pTexturePointer;
			};

			//! sort on distance (center) to camera
			struct TransparentModelEntry
			{
				TransparentModelEntry(IModel* m, const core::vector3df& camera)
					: m_pModel(m)
				{
					//TODO 根据位置进行判断不够精确，可能导致部分半透明模型显示错乱
					m_fDistance = m_pModel->getAbsoluteTransformation().getTranslation().getDistanceFromSQ(camera);
				}

				bool operator < (const TransparentModelEntry& other) const
				{
					//可以访问other的private成员？
					return m_fDistance > other.m_fDistance;
				}

				IModel* m_pModel;
			private:
				f64 m_fDistance;
			};

			ENUM_SCENE_PASS m_renderingPass;
			core::vector3df m_cameraPosition;
			camera::ICamera* m_activeCamera;

			//! render pass lists
			core::array<camera::ICamera*> m_cameras;
			core::array<DefaultModelEntry> m_solids;
			core::array<TransparentModelEntry> m_transparents;
			core::array<TransparentModelEntry> m_effects;

			ITimer* m_pTimer;
		public:
			CSceneManager(ITimer* timer);
			virtual ~CSceneManager();

			virtual IModel* addModel(IEntity* entity);
			virtual IModel* getRootModel(){
				return this;
			}

			virtual void clearModels(){
				clearChildren();
			}

			virtual terrain::ITerrainModel* addTerrainModel(IModel* parent,const core::vector3df& pos,
				const core::vector3df& rot,const core::vector3df& scale);

			virtual camera::ICamera* addCamera(camera::ENUM_CAMERA_TYPE cameraType,IModel* parent,
				const core::vector3df& pos,const core::vector3df& up,
				const core::vector3df& lookat,bool makeActive);
			virtual camera::ICamera* addCameraFPS(IModel* parent, f32 moveSpeed, event::SKeyMap* keyMapArray,s32 keyMapSize,
				const core::vector3df& pos,const core::vector3df& up,
				const core::vector3df& lookat,bool makeActive);
			virtual void setActiveCamera(camera::ICamera* camera);
			virtual camera::ICamera* getActiveCamera() const{return m_activeCamera;}

			virtual bool registerForRender(IModel* model,ENUM_SCENE_PASS pass);

			virtual void render(video::IVideoDriver* driver);

			virtual ENUM_SCENE_PASS getRenderingPass() const{
				return m_renderingPass;
			}

			virtual IGeometryFactory* getGeometryFactory() const;

			virtual animator::IAnimatorFactory* getAnimatorFactory() const;

			virtual void onResize(const core::dimension2du& size);

			virtual bool postEventFromUser(const event::SEvent& evt);
		private:
			IGeometryFactory* m_geometryFactory;
			animator::IAnimatorFactory* m_pAnimatorFactory;
		};
	}
}
#endif