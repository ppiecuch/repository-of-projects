#ifndef _YON_YONENGINE_H_
#define _YON_YONENGINE_H_


#include "config.h"
#include "EVideoType.h"
#include "IReferencable.h"


namespace yon{

	class YonEngine:public virtual core::IReferencable{
	public:
		//����
		//�������¼�ʱ��
		virtual void update() = 0;
	};
}

#endif