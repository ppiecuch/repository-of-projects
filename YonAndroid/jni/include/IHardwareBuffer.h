#ifndef _YON_VIDEO_IHARDWAREBUFFER_H_
#define _YON_VIDEO_IHARDWAREBUFFER_H_

#include "IReferencable.h"

namespace yon{
namespace video{

	enum ENUM_HARDWARDBUFFER_USAGE_TYPE{
		ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE = 0,	//不使用硬件缓冲
		ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC,		//数据可能会经常变化，使用更新开销不太大的方案
		ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC		//数据可能不太会变化，使用可以快速读取但不必快速更新的方案
	};

	enum ENUM_PRIMITIVE_TYPE;

	class IHardwareBuffer : public virtual core::IReferencable{
	public:
		virtual ~IHardwareBuffer(){}


		virtual void logon() = 0;
		virtual void logoff() = 0;

		virtual void draw(video::ENUM_PRIMITIVE_TYPE pType) = 0;
	};
}
}
#endif