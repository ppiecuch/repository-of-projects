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

	/*
	#define GL_POINTS                         0x0000
	#define GL_LINES                          0x0001
	#define GL_LINE_LOOP                      0x0002
	#define GL_LINE_STRIP                     0x0003
	#define GL_TRIANGLES                      0x0004
	#define GL_TRIANGLE_STRIP                 0x0005
	#define GL_TRIANGLE_FAN                   0x0006
	*/
	enum ENUM_PRIMITIVE_TYPE{
		ENUM_PRIMITIVE_TYPE_POINTS = 0x0000,
		ENUM_PRIMITIVE_TYPE_LINES = 0x0001,
		ENUM_PRIMITIVE_TYPE_TRIANGLES = 0x0004,
		ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP = 0x0005,
		ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN =0x0006,
		ENUM_PRIMITIVE_TYPE_COUNT = 1
	};

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