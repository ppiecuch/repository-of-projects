#ifndef _YON_VIDEO_IHARDWAREBUFFER_H_
#define _YON_VIDEO_IHARDWAREBUFFER_H_

#include "IReferencable.h"

namespace yon{
namespace video{

	enum ENUM_HARDWARDBUFFER_USAGE_TYPE{
		ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE = 0,	//��ʹ��Ӳ������
		ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC,		//���ݿ��ܻᾭ���仯��ʹ�ø��¿�����̫��ķ���
		ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC		//���ݿ��ܲ�̫��仯��ʹ�ÿ��Կ��ٶ�ȡ�����ؿ��ٸ��µķ���
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