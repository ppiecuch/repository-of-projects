#ifndef _YON_GUI_TYPECONVERTER_H_
#define _YON_GUI_TYPECONVERTER_H_

#include "ILogger.h"

namespace yon{
namespace gui{

	//refer to:http://stackoverflow.com/questions/9175257/why-comment-parameter-names-rather-than-leave-it-as-it-is
	//局部特化将优先于主模板
	//完全特化又会优先于局部特化

	/**
	Templatised TypeConverters with Template Specialisation.

	These converters convert from source types to destination types.
	They're mainly useful in things like dictionaries and serialisers.
	*/

	template <typename SourceType, typename DestType>
	class TypeConverter 
	{
	public:	
		static bool convert(const SourceType& src, DestType& dest);
	};

	//TODO why define this,is it applicable to some situation?
	//there are/were some compilers which issued a 'argument not used' warning when, well, the arguments were not used.
	//Especially in message handler functions like the one you showed, the arguments are often not necessary to perform the task. 
	//So by commenting the argument names out, you could get rid of the warnings.
	template <typename SourceType, typename DestType>
	bool TypeConverter<SourceType, DestType>::convert(const SourceType& /*src*/, DestType& /*dest*/)
	{
		YON_WARN(YON_LOG_WARN_FORMAT,"No converter specified!");
		return false;
	}

	//TODO
#if 0
	///
	/// Partial specialisations at the top, as they full specialisations should be prefered.
	///
	template< typename T >
	class TypeConverter< T, Stream >
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
	class TypeConverter< Stream, T >
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

#define YON_GUI_BASIC_CONVERTER(s, d) \
	template<>	\
	class TypeConverter< s, d > \
	{ \
	public: \
		static bool convert(const s& src, d& dest) \
		{ \
			dest = (d)src; \
			return true; \
		} \
	}

#define YON_GUI_BASIC_CONVERTER_BOOL(s, d) \
	template<>	\
	class TypeConverter< s, d > \
	{ \
	public: \
		static bool convert(const s& src, d& dest) \
		{ \
			dest = src != 0; \
			return true; \
		} \
	}

#define YON_GUI_PASS_THROUGH(t)	YON_GUI_BASIC_CONVERTER(t, t)

	/////////////////////////////////////////////////
	// Simple pass through definitions for converting 
	// to the same type (direct copy)
	/////////////////////////////////////////////////
	YON_GUI_PASS_THROUGH(s32);
	YON_GUI_PASS_THROUGH(u32);
	YON_GUI_PASS_THROUGH(f32);
	YON_GUI_PASS_THROUGH(bool);
	YON_GUI_PASS_THROUGH(c8);
	YON_GUI_PASS_THROUGH(u16);
	YON_GUI_PASS_THROUGH(core::vector2di);
	YON_GUI_PASS_THROUGH(core::vector2df);
	YON_GUI_PASS_THROUGH(video::SColorf);
	YON_GUI_PASS_THROUGH(video::SColor);
	YON_GUI_PASS_THROUGH(core::stringc);

	// Pointer types need to be typedef'd
	//TODO
	//class ScriptInterface;
	//typedef ScriptInterface* ScriptInterfacePtr;
	//YON_GUI_PASS_THROUGH(ScriptInterfacePtr);

	typedef void* voidPtr;
	YON_GUI_PASS_THROUGH(voidPtr);

	template<>
	class TypeConverter< Stream, Stream >
	{
	public:
		static bool Convert(Stream* src, Stream* dest)
		{
			return src->Write(dest, src->Length()) == src->Length();
		}
	};

	/////////////////////////////////////////////////
	// Simple Types
	/////////////////////////////////////////////////
	YON_GUI_BASIC_CONVERTER(bool, int);
	YON_GUI_BASIC_CONVERTER(bool, unsigned int);
	YON_GUI_BASIC_CONVERTER(bool, float);

	YON_GUI_BASIC_CONVERTER(int, unsigned int);
	YON_GUI_BASIC_CONVERTER_BOOL(int, bool);
	YON_GUI_BASIC_CONVERTER(int, float);

	YON_GUI_BASIC_CONVERTER_BOOL(float, bool);
	YON_GUI_BASIC_CONVERTER(float, int);
	YON_GUI_BASIC_CONVERTER(float, unsigned int);

	YON_GUI_BASIC_CONVERTER(char, word);

	/////////////////////////////////////////////////
	// From string converters
	/////////////////////////////////////////////////

#define YON_GUI_STRING_FLOAT_CONVERTER(type) \
	template<> \
	class TypeConverter< core::stringc, type > \
	{ \
	public: \
		static bool convert(const core::stringc& src, type& dest) \
		{ \
			dest = (type) src.superatof(); \
			return true; \
		} \
	};
	YON_GUI_STRING_FLOAT_CONVERTER(f32);
	YON_GUI_STRING_FLOAT_CONVERTER(f64);

	template<>
	class TypeConverter< core::stringc, s32 >
	{
	public:
		static bool convert(const core::stringc& src, s32& dest)
		{
			return sscanf(src.c_str(), "%d", &dest) == 1;
		}
	};

	template<>
	class TypeConverter< core::stringc, u32 >
	{
	public:
		static bool convert(const core::stringc& src, u32& dest)
		{
			return sscanf(src.c_str(), "%u", &dest) == 1;
		}
	};

	template<>
	class TypeConverter< core::stringc, u8 >
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
	class TypeConverter< core::stringc, bool >
	{
	public:
		static bool convert(const core::stringc& src, bool& dest)
		{
			core::stringc lower = src;
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

	template<>
	class TypeConverter< String, URL >
	{
	public:
		static bool Convert(const String& src, URL& dest)
		{
			return dest.SetURL(src);		
		}
	};

	template< typename DestType, typename InternalType, int count >
	class TypeConverterStringVector
	{
	public:
		static bool Convert(const String& src, DestType& dest)
		{
			StringList string_list;
			StringUtilities::ExpandString(string_list, src);
			if (string_list.size() < count)
				return false;
			for (int i = 0; i < count; i++)
			{
				if (!TypeConverter< String, InternalType >::Convert(string_list[i], dest[i]))
					return false;
			}
			return true;
		}
	};

#define STRING_VECTOR_CONVERTER(type, internal_type, count) \
	template<> \
	class TypeConverter< String, type > \
	{ \
	public: \
	static bool Convert(const String& src, type& dest) \
	{ \
	return TypeConverterStringVector< type, internal_type, count >::Convert(src, dest); \
	} \
	}

	STRING_VECTOR_CONVERTER(Vector2i, int, 2);
	STRING_VECTOR_CONVERTER(Vector2f, float, 2);
	STRING_VECTOR_CONVERTER(Colourf, float, 4);
	STRING_VECTOR_CONVERTER(Colourb, byte, 4);

	/////////////////////////////////////////////////
	// To String Converters
	/////////////////////////////////////////////////

#define FLOAT_STRING_CONVERTER(type) \
	template<> \
	class TypeConverter< type, String > \
	{ \
	public: \
	static bool Convert(const type& src, String& dest) \
	{ \
	return dest.FormatString(32, "%.4f", src) > 0; \
	} \
	};
	FLOAT_STRING_CONVERTER(float);
	FLOAT_STRING_CONVERTER(double);

	template<>
	class TypeConverter< int, String >
	{
	public:
		static bool Convert(const int& src, String& dest)
		{
			return dest.FormatString(32, "%d", src) > 0;
		}
	};

	template<>
	class TypeConverter< unsigned int, String >
	{
	public:
		static bool Convert(const unsigned int& src, String& dest)
		{
			return dest.FormatString(32, "%u", src) > 0;
		}
	};

	template<>
	class TypeConverter< byte, String >
	{
	public:
		static bool Convert(const byte& src, String& dest)
		{
			return dest.FormatString(32, "%u", src) > 0;
		}
	};

	template<>
	class TypeConverter< bool, String >
	{
	public:
		static bool Convert(const bool& src, String& dest)
		{
			dest = src ? "1" : "0";
			return true;
		}
	};

	template<>
	class TypeConverter< char*, String >
	{
	public:
		static bool Convert(char* const & src, String& dest)
		{
			dest = src;
			return true;
		}
	};

	template<>
	class TypeConverter< URL, String >
	{
	public:
		static bool Convert(const URL& src, String& dest)
		{
			dest = src.GetURL();
			return true;
		}
	};

	template< typename SourceType, typename InternalType, int count >
	class TypeConverterVectorString
	{
	public:
		static bool convert(const SourceType& src, core::stringc& dest)
		{
			dest = "";
			for (int i = 0; i < count; i++)
			{
				core::stringc value;
				if (!TypeConverter< InternalType, core::stringc >::convert(src[i], value))
					return false;

				dest += value;
				if (i < count - 1)
					dest += ", ";
			}
			return true;
		}
	};

#define VECTOR_STRING_CONVERTER(type, internal_type, count) \
	template<> \
	class TypeConverter< type, core::stringc > \
	{ \
	public: \
		static bool convert(const type& src, core::stringc& dest) \
		{ \
			return TypeConverterVectorString< type, internal_type, count >::convert(src, dest); \
		} \
	}

	VECTOR_STRING_CONVERTER(core::vector2di, s32, 2);
	VECTOR_STRING_CONVERTER(core::vector2df, f32, 2);
	VECTOR_STRING_CONVERTER(video::SColorf, f32, 4);
	VECTOR_STRING_CONVERTER(video::SColor, s32, 1);
#undef PASS_THROUGH
#undef BASIC_CONVERTER
#undef BASIC_CONVERTER_BOOL
#undef STRING_VECTOR_CONVERTER
#undef VECTOR_STRING_CONVERTER



}
}
#endif