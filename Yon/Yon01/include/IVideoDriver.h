#ifndef _YON_VIDEO_IVIDEODRIVER_H_
#define _YON_VIDEO_IVIDEODRIVER_H_

#include "IReferencable.h"
#include "color.h"
#include "rect.h"
#include "matrix4.h"
#include "IUnit.h"

namespace yon{
	namespace video{

		enum ENUM_TRANSFORM{
			ENUM_TRANSFORM_VIEW = 0,		//视图空间矩阵
			ENUM_TRANSFORM_WORLD,			//世界空间矩阵
			ENUM_TRANSFORM_PROJECTION,		//投影空间矩阵
			ENUM_TRANSFORM_COUNT
		};

		//视频驱动器接口
		class IVideoDriver:public virtual yon::core::IReferencable{
		protected:
			enum ENUM_RENDER_MODE{
				ENUM_RENDER_MODE_NONE = 0,
				ENUM_RENDER_MODE_2D,
				ENUM_RENDER_MODE_3D,
				ENUM_RENDER_MODE_COUNT
			};
			ENUM_RENDER_MODE m_renderMode;
		public:
			IVideoDriver():m_renderMode(ENUM_RENDER_MODE_NONE){}
			virtual ~IVideoDriver(){};
			virtual void begin(bool zBuffer=true,yon::core::color c=yon::core::color(0x000000FF)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const yon::core::recti& rec) = 0;

			virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat) =0;
			virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const =0;

			virtual void drawUnit(scene::IUnit* unit) const = 0;
			//virtual u32 getFPS() const = 0;
		};
	}
}
#endif