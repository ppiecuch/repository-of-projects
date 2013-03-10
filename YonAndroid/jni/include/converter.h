#ifndef _YON_CORE_CONVERTER_H_
#define _YON_CORE_CONVERTER_H_

#include "ILogger.h"

namespace yon{
namespace core{

	//refer to:http://stackoverflow.com/questions/9175257/why-comment-parameter-names-rather-than-leave-it-as-it-is
	//局部特化将优先于主模板
	//完全特化又会优先于局部特化

	/**
	Templatised convertor with Template Specialisation.

	These converters convert from source types to destination types.
	They're mainly useful in things like dictionaries and serialisers.
	*/

	template <typename SourceType, typename DestType>
	class convertor 
	{
	public:	
		static bool convert(const SourceType& src, DestType& dest);
	};

	//TODO why define this,is it applicable to some situation?难道是用于检测转换是否有进行特化？
	//there are/were some compilers which issued a 'argument not used' warning when, well, the arguments were not used.
	//Especially in message handler functions like the one you showed, the arguments are often not necessary to perform the task. 
	//So by commenting the argument names out, you could get rid of the warnings.
	template <typename SourceType, typename DestType>
	bool convertor<SourceType, DestType>::convert(const SourceType& /*src*/, DestType& /*dest*/)
	{
		YON_WARN(YON_LOG_WARN_FORMAT,"Unexpected convert!");
		return false;
	}

	//TODO
#if 0
	///
	/// Partial specialisations at the top, as they full specialisations should be prefered.
	///
	template< typename T >
	class convertor< T, Stream >
	{
	public:
		static bool Convert(const T& src, Stream* dest)
		{
			String string_dest;
			bool result = TypeConverter< T, String >::Convert(src, string_dest);
			if (result)
				dest->Write(string_dest);

			return result;
		}
	};

	template< typename T >
	class convertor< Stream, T >
	{
	public:
		static bool Convert(Stream* src, T& dest, size_t length = String::npos)
		{
			String string_src;
			src->Read(string_src, src->Length() < length ? src->Length() : length);
			return TypeConverter< String, T >::Convert(string_src, dest);
		}
	};
#endif

	///
	/// Full Specialisations
	///

#define YON_BASIC_CONVERTER(s, d) \
	template<>	\
	class convertor< s, d > \
	{ \
	public: \
		static bool convert(const s& src, d& dest) \
		{ \
			dest = (d)src; \
			return true; \
		} \
	}

#define YON_BASIC_CONVERTER_BOOL(s, d) \
	template<>	\
	class convertor< s, d > \
	{ \
	public: \
		static bool convert(const s& src, d& dest) \
		{ \
			dest = src != 0; \
			return true; \
		} \
	}

	/////////////////////////////////////////////////
	// From string converters
	/////////////////////////////////////////////////

#define YON_STRING_FLOAT_CONVERTER(type) \
	template<> \
	class convertor< core::stringc, type > \
	{ \
	public: \
		static bool convert(const core::stringc& src, type& dest) \
		{ \
			dest = (type) src.superatof(); \
			return true; \
		} \
	};

#if 0
	template<>
	class convertor< Stream, Stream >
	{
	public:
		static bool Convert(Stream* src, Stream* dest)
		{
			return src->Write(dest, src->Length()) == src->Length();
		}
	};
#endif

#define YON_PASS_THROUGH(t)	YON_BASIC_CONVERTER(t, t)

	/////////////////////////////////////////////////
	// Simple pass through definitions for converting 
	// to the same type (direct copy)
	/////////////////////////////////////////////////
	typedef void* voidPtr;
	YON_PASS_THROUGH(voidPtr);
	YON_PASS_THROUGH(s32);
	YON_PASS_THROUGH(u32);
	YON_PASS_THROUGH(f32);
	YON_PASS_THROUGH(bool);
	YON_PASS_THROUGH(c8);
	YON_PASS_THROUGH(u16);
	YON_PASS_THROUGH(core::vector2di);
	YON_PASS_THROUGH(core::vector2df);
	YON_PASS_THROUGH(video::SColorf);
	YON_PASS_THROUGH(video::SColor);
	YON_PASS_THROUGH(core::stringc);

	// Pointer types need to be typedef'd
	//TODO
	//class ScriptInterface;
	//typedef ScriptInterface* ScriptInterfacePtr;
	//YON_PASS_THROUGH(ScriptInterfacePtr);


	/////////////////////////////////////////////////
	// Simple Types
	/////////////////////////////////////////////////
	YON_BASIC_CONVERTER(bool, s32);
	YON_BASIC_CONVERTER(bool, u32);
	YON_BASIC_CONVERTER(bool, f32);

	YON_BASIC_CONVERTER(c8, u16);

	YON_BASIC_CONVERTER(s32, u32);
	YON_BASIC_CONVERTER_BOOL(s32, bool);
	YON_BASIC_CONVERTER(s32, f32);

	YON_BASIC_CONVERTER_BOOL(f32, bool);
	YON_BASIC_CONVERTER(f32, s32);
	YON_BASIC_CONVERTER(f32, u32);


	/////////////////////////////////////////////////
	// String to Others Convert
	/////////////////////////////////////////////////

	YON_STRING_FLOAT_CONVERTER(f32);
	YON_STRING_FLOAT_CONVERTER(f64);

	template<>
	class convertor< core::stringc, s32 >
	{
	public:
		static bool convert(const core::stringc& src, s32& dest)
		{
			return sscanf(src.c_str(), "%d", &dest) == 1;
		}
	};

	template<>
	class convertor< core::stringc, u32 >
	{
	public:
		static bool convert(const core::stringc& src, u32& dest)
		{
			return sscanf(src.c_str(), "%u", &dest) == 1;
		}
	};

	template<>
	class convertor< core::stringc, u8 >
	{
	public:
		static bool convert(const core::stringc& src, u8& dest)
		{
			s32 value;
			bool ret = sscanf(src.c_str(), "%d", &value) == 1;
			dest = (u8) value;
			return ret && (value <= 255);
		}
	};

	template<>
	class convertor< core::stringc, bool >
	{
	public:
		static bool convert(const core::stringc& src, bool& dest)
		{
			core::stringc lower(src);
			lower.makeLower();
			if (lower == "1" || lower == "true")
			{
				dest = true;
				return true;
			}
			else if (lower == "0" || lower == "false")
			{
				dest = false;
				return true;
			}
			return false;
		}
	};

#if 0
	template<>
	class convertor< String, URL >
	{
	public:
		static bool convert(const String& src, URL& dest)
		{
			return dest.SetURL(src);		
		}
	};
#endif

	template< typename DestType, typename InternalType, s32 count >
	class ConverterStringArray
	{
	public:
		static bool convert(const core::stringc& src, DestType& dest)
		{
			core::array<core::stringc> arr;
			src.split(arr," ");
			if (arr.size() < count)
			{
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("ConverterStringArray:convert %s failed,for no request count:%d!",src.c_str(),count).c_str());
				return false;
			}
			for (s32 i = 0; i < count; i++)
			{
				if (!convertor< core::stringc, InternalType >::convert(arr[i], dest[i]))
					return false;
			}
			return true;
		}
	};

#define YON_STRING_ARRAY_CONVERTER(type, internal_type, count) \
	template<> \
	class convertor< core::stringc, type > \
	{ \
	public: \
		static bool convert(const core::stringc& src, type& dest) \
		{ \
			return ConverterStringArray< type, internal_type, count >::convert(src, dest); \
		} \
	}

	YON_STRING_ARRAY_CONVERTER(core::vector2di, s32, 2);
	YON_STRING_ARRAY_CONVERTER(core::vector2df, f32, 2);
	YON_STRING_ARRAY_CONVERTER(video::SColorf, f32, 4);
	YON_STRING_ARRAY_CONVERTER(video::SColor, u8, 4);

	/////////////////////////////////////////////////
	// Others to String Convert
	/////////////////////////////////////////////////
#define YON_FLOAT_STRING_CONVERTER(type) \
	template<> \
	class convertor< type, core::stringc > \
	{ \
	public: \
		static bool convert(const type& src, core::stringc& dest) \
		{ \
			return dest.format<ENUM_SIZE_128>("%.4f", src) > 0; \
		} \
	};
	YON_FLOAT_STRING_CONVERTER(float);
	YON_FLOAT_STRING_CONVERTER(double);

	template<>
	class convertor< s32, core::stringc >
	{
	public:
		static bool convert(const s32& src, core::stringc& dest)
		{
			return dest.format<ENUM_SIZE_128>("%d", src) > 0;
		}
	};

	template<>
	class convertor< u32, core::stringc >
	{
	public:
		static bool convert(const u32& src, core::stringc& dest)
		{
			return dest.format<ENUM_SIZE_128>("%u", src) > 0;
		}
	};

	template<>
	class convertor< u8, core::stringc >
	{
	public:
		static bool convert(const u8& src, core::stringc& dest)
		{
			return dest.format<ENUM_SIZE_128>("%u", src) > 0;
		}
	};

	template<>
	class convertor< bool, core::stringc >
	{
	public:
		static bool convert(const bool& src, core::stringc& dest)
		{
			//TODO 为何不是true/false
			dest = src ? "1" : "0";
			return true;
		}
	};

	template<>
	class convertor< c8*, core::stringc >
	{
	public:
		//TODO 为何是c8* const而不是const c8*
		static bool convert(c8* const & src, core::stringc& dest)
		{
			dest = src;
			return true;
		}
	};

#if 0
	template<>
	class convertor< URL, String >
	{
	public:
		static bool Convert(const URL& src, String& dest)
		{
			dest = src.GetURL();
			return true;
		}
	};
#endif
	template< typename SourceType, typename InternalType, int count >
	class ConverterArrayString
	{
	public:
		static bool convert(const SourceType& src, core::stringc& dest)
		{
			dest = "";
			for (int i = 0; i < count; i++)
			{
				core::stringc value;
				if (!convertor< InternalType, core::stringc >::convert(src[i], value))
					return false;

				dest += value;
				if (i < count - 1)
					dest += ", ";
			}
			return true;
		}
	};

#define YON_ARRAY_STRING_CONVERTER(type, internal_type, count) \
	template<> \
	class convertor< type, core::stringc > \
	{ \
	public: \
		static bool convert(const type& src, core::stringc& dest) \
		{ \
			return ConverterArrayString< type, internal_type, count >::convert(src, dest); \
		} \
	}

	YON_ARRAY_STRING_CONVERTER(core::vector2di, s32, 2);
	YON_ARRAY_STRING_CONVERTER(core::vector2df, f32, 2);
	//TODO Color到字符串的转换（浮点/定点/16进制）
	YON_ARRAY_STRING_CONVERTER(video::SColorf, f32, 4);
	YON_ARRAY_STRING_CONVERTER(video::SColor, s32, 1);

#undef YON_PASS_THROUGH
#undef YON_BASIC_CONVERTER
#undef YON_BASIC_CONVERTER_BOOL
#undef YON_STRING_ARRAY_CONVERTER
#undef YON_ARRAY_STRING_CONVERTER



}
}
#endif