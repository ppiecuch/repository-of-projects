#ifndef _YON_CORE_IRENDERABLE_H_
#define _YON_CORE_IRENDERABLE_H_

#include "IReferencable.h"
#include "IVideoDriver.h"
#include "vector3d.h"

namespace yon{
	namespace core{

		class IRenderable : public virtual IReferencable{
		protected:
			core::vector3df m_position;
		public:
			IRenderable(const core::vector3df& pos=core::vector3df(0,0,0)):m_position(pos){}
			virtual void render(video::IVideoDriver* driver) = 0;

			virtual void setPosition(const core::vector3df& pos){
				m_position=pos;
			}
			virtual const core::vector3df& getPosition() const{
				return m_position;
			}
		};
	}
}
#endif