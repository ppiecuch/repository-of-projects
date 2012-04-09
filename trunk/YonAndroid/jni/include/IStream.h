#ifndef _YON_IO_ISTREAM_H_
#define _YON_IO_ISTREAM_H_

#include "IReferencable.h"

namespace yon{
namespace io{

	enum ENDIAN_MODE{
		ENDIAN_MODE_BIG = 0,	//���ģʽ
		ENDIAN_MODE_LITTLE		//С��ģʽ
	};

	enum STREAM_TYPE{
		STREAM_TYPE_FILE = 0,	//�ļ���
		STREAM_TYPE_MEMORY,		//�ڴ�������
		STREAM_TYPE_NET,		//������
		STREAM_TYPE_COUNT
	};

		
	class IReadStream : public virtual core::IReferencable{
	protected:
		ENDIAN_MODE m_endianMode;
		STREAM_TYPE m_streamType;

		virtual readData(void* data,u32 num) = 0;

		template <typename T> T read()
		{
			T value;
			readData(&value, sizeof(value));
			return value;
		}
	public:
		IReadStream(STREAM_TYPE type,ENDIAN_MODE mode=ENDIAN_MODE_LITTLE): m_streamType(type),m_endianMode(mode){}
		STREAM_TYPE getType() const{return m_streamType;}
		ENDIAN_MODE getEndianMode() const{return m_endianMode;}

		//offset:ƫ����
		//relative:����ڵ�ǰλ��
		virtual bool seek(long offset, bool relative = false) = 0;

		//��ǰλ�ã����ֽ�Ϊ��λ
		virtual long getPos() const = 0;

		virtual s32 read(void* buffer, u32 sizeToRead) = 0;

		
		inline bool readBool(){return readByte()!=0;}
		inline s8 readByte(){return read<s8>();}
		inline u8 readUnsignedByte(){return read<u8>();}
		inline s16 readShort(){return read<s16>();}
		inline u16 readUnsignedShort(){return read<u16>();}
		inline s32 readInt(){return read<s32>();}
		inline u32 readUnsignedInt(){return read<u32>();}
		inline s64 readLong(){return read<s64>();}
		inline u64 readUnsignedLong(){return read<u64>();}
		inline f32 readFloat(){return read<f32>();}
		inline f64 readDouble(){return read<f64>();}

	};
}
}
#endif