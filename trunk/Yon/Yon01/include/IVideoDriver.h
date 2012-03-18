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
			ENUM_TRANSFORM_VIEW = 0,		//��ͼ�ռ����
			ENUM_TRANSFORM_WORLD,			//����ռ����
			ENUM_TRANSFORM_PROJECTION,		//ͶӰ�ռ����
			ENUM_TRANSFORM_COUNT
		};

		//��Ƶ�������ӿ�
		class IVideoDriver:public virtual yon::core::IReferencable{
		public:
			virtual ~IVideoDriver(){};
			virtual void begin(bool zBuffer=true,yon::core::color c=yon::core::color(0x000000FF)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const yon::core::recti& rec) = 0;

			virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat) =0;
			virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const =0;

			virtual void drawUnit(const scene::IUnit* unit) const = 0;
			//virtual u32 getFPS() const = 0;
		};
	}
}
#endif