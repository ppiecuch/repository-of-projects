#ifndef _YON_CORE_CONVERTER_H_
#define _YON_CORE_CONVERTER_H_

#include "ILogger.h"
#include "yonArray.h"

namespace yon{
namespace core{

	template <typename SourceType, typename DestType>
	class convertor 
	{
	public:	
		static bool convert(const SourceType& src, DestType& dest);
	};

	template <typename SourceType, typename DestType>
	bool convertor<SourceType, DestType>::convert(const SourceType& /*src*/, DestType& /*dest*/)
	{
		YON_WARN(YON_LOG_WARN_FORMAT,"Unexpected convert!");
		return false;
	}

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
	// Same Types Convertion
	/////////////////////////////////////////////////
#define YON_PASS_THROUGH(t)	YON_BASIC_CONVERTER(t, t)

	YON_PASS_THROUGH(pvoid);
	YON_PASS_THROUGH(s32);
	YON_PASS_THROUGH(u32);
	YON_PASS_THROUGH(f32);
	YON_PASS_THROUGH(bool);
	YON_PASS_THROUGH(c8);
	YON_PASS_THROUGH(u16);
	//YON_PASS_THROUGH(core::vector2di);
	YON_PASS_THROUGH(core::vector2df);
	YON_PASS_THROUGH(video::SColorf);
	YON_PASS_THROUGH(video::SColor);
	YON_PASS_THROUGH(core::stringc);

#undef YON_PASS_THROUGH



	/////////////////////////////////////////////////
	// Basic Types Conversion
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

#undef YON_BASIC_CONVERTER
#undef YON_BASIC_CONVERTER_BOOL



	/////////////////////////////////////////////////
	// String To Othre Types Conversion
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

	YON_STRING_FLOAT_CONVERTER(f32);
	YON_STRING_FLOAT_CONVERTER(f64);

	template<>
	class convertor< core::stringc, s32 >
	{
	public:
		static bool convert(const core::stringc& src, s32& dest)
		{
			return sscanf_s(src.c_str(), "%d", &dest,src.length()) == 1;
		}
	};

	template<>
	class convertor< core::stringc, u32 >
	{
	public:
		static bool convert(const core::stringc& src, u32& dest)
		{
			return sscanf_s(src.c_str(), "%u", &dest ,src.length()) == 1;
		}
	};

	template<>
	class convertor< core::stringc, u8 >
	{
	public:
		static bool convert(const core::stringc& src, u8& dest)
		{
			s32 value;
			bool ret = sscanf_s(src.c_str(), "%d", &value,src.length()) == 1;
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

	template<>
	class convertor< core::stringc, core::vector2df >
	{
	public:
		static bool convert(const core::stringc& src, core::vector2df& dest)
		{
			core::array<core::stringc> arr;
			src.split(arr," ");
			if (arr.size() != 2)
			{
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("ConverterStringArray:convert %s failed,for no request count:2!",src.c_str()).c_str());
				return false;
			}
			if (!convertor< core::stringc, f32 >::convert(arr[0], dest.x))
				return false;
			if (!convertor< core::stringc, f32 >::convert(arr[1], dest.y))
				return false;
			return true;
		}
	};

	template<>
	class convertor< core::stringc, video::SColor >
	{
	public:
		static bool convert(const core::stringc& src, video::SColor& dest)
		{
			dest=video::SColor::fromHexString(src);
			return true;
		}
	};

	template<>
	class convertor< core::stringc, video::SColorf >
	{
	public:
		static bool convert(const core::stringc& src, video::SColorf& dest)
		{
			dest=video::SColor::fromHexString(src);
			return true;
		}
	};

	/////////////////////////////////////////////////
	// Others Type To String Conversion
	/////////////////////////////////////////////////
#define YON_FLOAT_STRING_CONVERTER(type) \
	template<> \
	class convertor< type, core::stringc > \
	{ \
	public: \
		static bool convert(const type& src, core::stringc& dest) \
		{ \
			dest.format<ENUM_SIZE_32>("%.4f", src); \
			return true; \
		} \
	}
	YON_FLOAT_STRING_CONVERTER(f32);
	YON_FLOAT_STRING_CONVERTER(f64);

#undef YON_FLOAT_STRING_CONVERTER

	template<>
	class convertor< s32, core::stringc >
	{
	public:
		static bool convert(const s32& src, core::stringc& dest)
		{
			dest.format<ENUM_SIZE_16>("%d", src);
			return true;
		}
	};

	template<>
	class convertor< u32, core::stringc >
	{
	public:
		static bool convert(const u32& src, core::stringc& dest)
		{
			dest.format<ENUM_SIZE_16>("%u", src);
			return true;
		}
	};

	template<>
	class convertor< u8, core::stringc >
	{
	public:
		static bool convert(const u8& src, core::stringc& dest)
		{
			dest.format<ENUM_SIZE_8>("%u", src);
			return true;
		}
	};

	template<>
	class convertor< bool, core::stringc >
	{
	public:
		static bool convert(const bool& src, core::stringc& dest)
		{
			dest = src ? "true" : "false";
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

	template<>
	class convertor< core::vector2df, core::stringc >
	{
	public:
		static bool convert(const core::vector2df& src, core::stringc& dest)
		{
			dest.format<ENUM_SIZE_32>("%.4f %.4f",src.x,src.y);
			return true;
		}
	};

	template<>
	class convertor< video::SColor, core::stringc >
	{
	public:
		static bool convert(const video::SColor& src, core::stringc& dest)
		{
			dest=src.toHexString();
			return true;
		}
	};

	template<>
	class convertor< video::SColorf, core::stringc >
	{
	public:
		static bool convert(const video::SColorf& src, core::stringc& dest)
		{
			dest=src.toSColor().toHexString();
			return true;
		}
	};
}
}
#endif