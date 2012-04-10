#ifndef _YON_IO_ISTREAM_H_
#define _YON_IO_ISTREAM_H_

#include "IReferencable.h"
#include "path.h"

namespace yon{
namespace io{

	enum ENUM_ENDIAN_MODE{
		ENUM_ENDIAN_MODE_BIG = 0,		//���ģʽ
		ENUM_ENDIAN_MODE_LITTLE			//С��ģʽ
	};

	enum ENUM_STREAM_TYPE{
		ENUM_STREAM_TYPE_FILE = 0,		//�ļ���
		ENUM_STREAM_TYPE_MEMORY,		//�ڴ�������
		ENUM_STREAM_TYPE_NET,			//������
		ENUM_STREAM_TYPE_COUNT
	};

		
	class IReadStream : public virtual core::IReferencable{
	protected:
		io::path m_path;
		ENUM_ENDIAN_MODE m_endianMode;
		ENUM_STREAM_TYPE m_streamType;

		virtual void readDataInEndianMode(void* data,u32 sizeToRead) = 0;

		template <typename T> T read()
		{
			T value;
			readDataInEndianMode(&value, sizeof(value));
			return value;
		}
	public:
		IReadStream(const io::path& name,ENUM_STREAM_TYPE type,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE): m_path(name),m_streamType(type),m_endianMode(mode){}
		virtual ~IReadStream(){}
		ENUM_STREAM_TYPE getType() const{return m_streamType;}
		ENUM_ENDIAN_MODE getEndianMode() const{return m_endianMode;}

		//offset:ƫ����
		//relative:����ڵ�ǰλ��
		virtual bool seek(long offset, bool relative = false) = 0;

		//���ض�ȡ���ֽ���
		virtual s32 read(u8* buffer, u32 sizeToRead) = 0;

		//��ǰλ�ã����ֽ�Ϊ��λ
		virtual u32 getPos() const = 0;

		//ʹ��ǰת��Ϊ��Ӧ�����ͣ����ļ���תΪ(FILE*)
		virtual void* pointer() const = 0;

		//��ȡ����С�����ֽ�Ϊ��λ
		virtual u32 getSize() const = 0;

		virtual const io::path& getPath() const{return m_path;}

		
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