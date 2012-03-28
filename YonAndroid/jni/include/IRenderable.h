#ifndef _YON_CORE_IRENDERABLE_H_
#define _YON_CORE_IRENDERABLE_H_

#include "IReferencable.h"
#include "vector3d.h"
#include "matrix4.h"
#include "IVideoDriver.h"

namespace yon{
	namespace core{

		class IRenderable : public virtual IReferencable{
		protected:
			bool m_bTransformationChanged;
			core::matrix4f m_transformation;
			core::vector3df m_position;
			core::vector3df m_rotation;
			core::vector3df m_scale;
		public:
			IRenderable(const core::vector3df& pos=core::vector3df(0,0,0),
				const core::vector3df& rot=core::vector3df(0,0,0),
				const core::vector3df& scale=core::vector3df(1,1,1)):
				m_position(pos),m_rotation(rot),m_scale(scale),
				m_bTransformationChanged(true){}
			virtual void render(video::IVideoDriver* driver) = 0;

			virtual void setPosition(const core::vector3df& pos){
				m_position=pos;
				m_bTransformationChanged=true;
			}
			virtual const core::vector3df& getPosition() const{
				return m_position;
			}

			virtual void setRotation(const core::vector3df& rotation){
				m_rotation=rotation;
				m_bTransformationChanged=true;
			}
			virtual const core::vector3df& getRotation() const{
				return m_rotation;
			}

			virtual void setScale(const core::vector3df& scale){
				m_scale=scale;
				m_bTransformationChanged=true;
			}
			virtual const core::vector3df& getScale() const{
				return m_scale;
			}

			virtual const core::matrix4f& getAbsoluteTransformation()
			{
				return getRelativeTransformation();
			}

			virtual const core::matrix4f& getRelativeTransformation()
			{
				if(m_bTransformationChanged){
					m_transformation.makeIdentity();
					m_transformation.setRotationDegrees(m_rotation); 
					m_transformation.setTranslation(m_position);
					if (m_scale != core::IDENTITY_VECTOR3DF)
					{
						m_transformation.scale(m_scale);
					}
					m_bTransformationChanged=false;
				}
				return m_transformation;
			}
		};
	}
}
#endif