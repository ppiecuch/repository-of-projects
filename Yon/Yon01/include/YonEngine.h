#ifndef _YON_YONENGINE_H_
#define _YON_YONENGINE_H_


#include "config.h"
#include "EVideoType.h"
#include "IReferencable.h"


namespace yon{

	class YonEngine:public virtual core::IReferencable{
	public:
		//更新
		//包括更新计时器
		virtual void update() = 0;
	};
}

#endif