#ifndef _YON_IO_ISTREAM_H_
#define _YON_IO_ISTREAM_H_

#include "IReferencable.h"
#include "path.h"
#ifdef YON_COMPILE_WITH_ANDROID
#include <string.h>
#endif
namespace yon{
namespace io{

	enum ENUM_ENDIAN_MODE{
		ENUM_ENDIAN_MODE_BIG = 0,		//大端模式
		ENUM_ENDIAN_MODE_LITTLE			//小端模式
	};

	enum ENUM_STREAM_TYPE{
		ENUM_STREAM_TYPE_FILE = 0,		//文件流
		ENUM_STREAM_TYPE_MEMORY,		//内存数据流
		ENUM_STREAM_TYPE_NET,			//网络流
		ENUM_STREAM_TYPE_COUNT
	};

	class IWriteStream : public virtual core::IReferencable{
	protected:
		io::path m_path;
		ENUM_ENDIAN_MODE m_endianMode;
		ENUM_STREAM_TYPE m_streamType;

		virtual void writeDataInEndianMode(void* data,u32 sizeToWrite) = 0;

		template <typename T> void write(T value)
		{
			writeDataInEndianMode(&value, sizeof(value));
		}
	public:
		IWriteStream(const io::path& name,ENUM_STREAM_TYPE type,ENUM_ENDIAN_MODE mode=ENUM_ENDIAN_MODE_LITTLE): m_path(name),m_streamType(type),m_endianMode(mode){}
		virtual ~IWriteStream(){}
		ENUM_STREAM_TYPE getType() const{return m_streamType;}
		void setEndianMode(ENUM_ENDIAN_MODE mode){m_endianMode=mode;}
		ENUM_ENDIAN_MODE getEndianMode() const{return m_endianMode;}
		virtual const io::path& getPath() const{return m_path;}

		//offset:偏移量
		//relative:相对于当前位置
		virtual bool seek(long offset, bool relative = false) = 0;

		//返回写入的字节数
		virtual s32 write(const u8* buffer, u32 sizeToWrite) = 0;

		//当前位置，以字节为单位
		virtual u32 getPos() const = 0;

		//使用前转换为对应的类型，如文件流转为(FILE*)
		virtual void* pointer() const = 0;

		inline void writeBool(bool value){ writeByte(value?true:false);}
		inline void writeByte(s8 value){ write<s8>(value);}
		inline void writeChar(c8 value){ write<c8>(value);}
		inline void writeUnsignedByte(u8 value){ write<u8>(value);}
		inline void writeShort(s16 value){ write<s16>(value);}
		inline void writeUnsignedShort(u16 value){ write<u16>(value);}
		inline void writeInt(s32 value){ write<s32>(value);}
		inline void writeUnsignedInt(u32 value){ write<u32>(value);}
		inline void writeLong(s64 value){ write<s64>(value);}
		inline void writeUnsignedLong(u64 value){ write<u64>(value);}
		inline void writeFloat(f32 value){ write<f32>(value);}
		inline void writeDouble(f64 value){ write<f64>(value);}
		inline void writeString(const core::stringc& str)
		{
			writeUnsignedInt(str.length());
			write((const u8*)(str.c_str()),str.length());
		}
		inline void writeString(const c8* str)
		{
			//core::stringc text(str);//直接创建会有“%”的问题
			core::stringc text;
			u32 size = 0;
			const c8* p = str;
			do
			{
				++size;
			} while(*p++);
			text.build(str,size);
			writeString(text);
		}
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
		void setEndianMode(ENUM_ENDIAN_MODE mode){m_endianMode=mode;}
		ENUM_ENDIAN_MODE getEndianMode() const{return m_endianMode;}
		virtual const io::path& getPath() const{return m_path;}

		//offset:偏移量
		//relative:相对于当前位置
		virtual bool seek(long offset, bool relative = false) = 0;

		//返回读取的字节数
		virtual s32 read(u8* buffer, u32 sizeToRead) = 0;

		//当前位置，以字节为单位
		virtual u32 getPos() const = 0;

		//使用前转换为对应的类型，如文件流转为(FILE*)
		virtual void* pointer() const = 0;

		//获取流大小，以字节为单位
		virtual u32 getSize() const = 0;

		
		inline bool readBool(){return readByte()!=0;}
		inline s8 readByte(){return read<s8>();}
		inline c8 readChar(){return read<c8>();}
		inline c16 readChar16(){return read<c16>();}
		inline c32 readChar32(){return read<c32>();}
		inline u8 readUnsignedByte(){return read<u8>();}
		inline s16 readShort(){return read<s16>();}
		inline u16 readUnsignedShort(){return read<u16>();}
		inline s32 readInt(){return read<s32>();}
		inline u32 readUnsignedInt(){return read<u32>();}
		inline s64 readLong(){return read<s64>();}
		inline u64 readUnsignedLong(){return read<u64>();}
		inline f32 readFloat(){return read<f32>();}
		inline f64 readDouble(){return read<f64>();}
		inline core::stringc readString()
		{
			static u8 buffer[65535];
			memset(buffer,0x0,65535);
			u32 len=readUnsignedInt();
			read(buffer,len);
			core::stringc str;
			str.build(buffer,len);
			return str;
		}

	};
}
}
#endif