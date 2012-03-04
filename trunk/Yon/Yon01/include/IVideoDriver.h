#ifndef _YON_VIDEO_IVIDEODRIVER_H_
#define _YON_VIDEO_IVIDEODRIVER_H_

#include "IReferencable.h"
#include "color.h"
#include "rect.h"

namespace yon{
	namespace video{

		enum E_MATRIX{
			E_MATRIX_VIEW = 0,		//视图空间矩阵
			E_MATRIX_WORLD,			//世界空间矩阵
			E_MATRIX_PROJECTION,	//投影空间矩阵
			E_MATRIX_COUNT
		};

		//视频驱动器接口
		class IVideoDriver:public virtual yon::core::IReferencable{
		public:
			virtual ~IVideoDriver(){};
			virtual void begin(bool zBuffer=true,yon::core::color c=yon::core::color(0x000000FF)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const yon::core::recti& rec) = 0;
			//virtual u32 getFPS() const = 0;
		};
	}
}
#endif