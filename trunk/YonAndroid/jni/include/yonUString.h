#ifndef _YON_CORE_YONUSTRING_H_
#define _YON_CORE_YONUSTRING_H_

//refer to:http://zh.wikipedia.org/zh-cn/UTF-16
//refer to:http://zh.wikipedia.org/zh-cn/UTF-32
//refer to:http://zh.wikipedia.org/wiki/%E4%BD%8D%E5%85%83%E7%B5%84%E9%A0%86%E5%BA%8F%E8%A8%98%E8%99%9F

#if (__cplusplus > 199711L) || (_MSC_VER >= 1600) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#	define USTRING_CPP0X
#	if defined(__GXX_EXPERIMENTAL_CXX0X__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)))
#		define USTRING_CPP0X_NEWLITERALS
#	endif
#endif

#ifdef USTRING_CPP0X
#	include <utility>
#endif

#include "yonArray.h"
#include "yonMath.h"
#include "yonString.h"
#include "path.h"


//! UTF-16 surrogate start values.
static const yon::u16 UTF16_HI_SURROGATE = 0xD800;
static const yon::u16 UTF16_LO_SURROGATE = 0xDC00;

//UTF-16与UCS-2的关系
//UTF-16可看成是UCS-2的父集。在没有辅助平面字符（surrogate code points）前，UTF-16与UCS-2所指的是同一的意思。
//但当引入辅助平面字符后，就称为UTF-16了。现在若有软件声称自己支持UCS-2编码，那其实是暗指它不能支持在UTF-16中超过2bytes的字集。
//对于小于0x10000的UCS码，UTF-16编码就等于UCS码。

//UTF-32 (或 UCS-4)是一种将Unicode字符编码的协定，对每一个Unicode码位使用恰好32位元。其它的Unicode transformation formats则使用不定长度编码。

//Unicode的码空间从U+0000到U+10FFFF，共有1,112,064个码位(code point)可用来映射字符.
//Unicode的码空间可以划分为17个平面(plane)，每个平面包含216(65,536)个码位。
//每个平面的码位可表示为从U+xx0000到U+xxFFFF, 其中xx表示十六进制值从0016 到1016，共计17个平面。
//第一个平面成为基本多文种平面（Basic Multilingual Plane, BMP），或称第零平面（Plane 0）。
//其他平面称为辅助平面(Supplementary Planes)。

//基本多语言平面内，从U+D800到U+DFFF之间的码位区段是永久保留不映射到字符，
//因此UTF-16利用保留下来的0xD800-0xDFFF区段的码位来对辅助平面的字符的码位进行编码。
//从U+0000至U+D7FF以及从U+E000至U+FFFF的码位进行UTF16_IS_SURROGATE都不可能成立

//高位的10比特的值(值的范围为0..0x3FF)被加上0xD800得到第一个码元或称作高位代理(high surrogate), 值的范围是0xD800..0xDBFF. 
//低位的10比特的值(值的范围也是0..0x3FF)被加上0xDC00得到第二个码元或称作低位代理(low surrogate), 值的范围是0xDC00..0xDFFF. 
//! Is a UTF-16 code point a surrogate?
#define UTF16_IS_SURROGATE(c)		(((c) & 0xF800) == 0xD800)
#define UTF16_IS_SURROGATE_HI(c)	(((c) & 0xFC00) == 0xD800)
#define UTF16_IS_SURROGATE_LO(c)	(((c) & 0xFC00) == 0xDC00)

namespace yon{

	// Define our character types.
#ifdef USTRING_CPP0X_NEWLITERALS	// C++0x
	typedef char32_t uchar32_t;
	typedef char16_t uchar16_t;
	typedef char uchar8_t;
#else
	typedef u32 uchar32_t;
	typedef u16 uchar16_t;
	typedef u8 uchar8_t;
#endif

	//! Unicode encoding type.
	enum ENUM_UTF_ENCODING
	{
		ENUM_UTF_ENCODING_NONE		= 0,
		ENUM_UTF_ENCODING_UTF8,
		ENUM_UTF_ENCODING_UTF16,
		ENUM_UTF_ENCODING_UTF16LE,
		ENUM_UTF_ENCODING_UTF16BE,
		ENUM_UTF_ENCODING_UTF32,
		ENUM_UTF_ENCODING_UTF32LE,
		ENUM_UTF_ENCODING_UTF32BE
	};

	//! Unicode endianness.
	enum ENUM_UTF_ENDIAN
	{
		ENUM_UTF_ENDIAN_NATIVE	= 0,
		ENUM_UTF_ENDIAN_LITTLE,
		ENUM_UTF_ENDIAN_BIG
	};

namespace core{

	namespace unicode{

		//! The unicode replacement character.  Used to replace invalid characters.
		const u16 UTF_REPLACEMENT_CHARACTER = 0xFFFD;

		//TODO 待理解
		//! Convert a UTF-16 surrogate pair into a UTF-32 character.
		//! \param high The high value of the pair.
		//! \param low The low value of the pair.
		//! \return The UTF-32 character expressed by the surrogate pair.
		inline uchar32_t toUTF32(uchar16_t high, uchar16_t low)
		{
			// Convert the surrogate pair into a single UTF-32 character.
			//总长20比特，高位占6位，低位占14位
			//相当于[(high&0x003F)<<10|(low&0x03FF)]
			uchar32_t x = ((high & ((1 << 6) -1)) << 10) | (low & ((1 << 10) -1));
			//相当于(high的高10位&0x001F)+1
			uchar32_t wu = ((high >> 6) & ((1 << 5) - 1)) + 1;
			return (wu << 16) | x;
		}

		//! Swaps the endianness of a 16-bit value.
		//! \return The new value.
		inline uchar16_t swapEndian16(const uchar16_t& c)
		{
			return ((c >> 8) & 0x00FF) | ((c << 8) & 0xFF00);
		}

		//! Swaps the endianness of a 32-bit value.
		//! \return The new value.
		inline uchar32_t swapEndian32(const uchar32_t& c)
		{
			return  ((c >> 24) & 0x000000FF) |
				((c >> 8)  & 0x0000FF00) |
				((c << 8)  & 0x00FF0000) |
				((c << 24) & 0xFF000000);
		}

		//字节顺序标记（英语：byte-order mark，BOM）是位于码点U+FEFF的统一码字符的名称。
		//当以UTF-16或UTF-32来将UCS/统一码字符所组成的字符串编码时，这个字符被用来标示其字节序。
		//它常被用来当做标示文件是以UTF-8、UTF-16或UTF-32编码的记号。

		//字符U+FEFF如果出现在字节流的开头，则用来标识该字节流的字节序，是高位在前还是低位在前。
		//如果它出现在字节流的中间，则表达零宽度非换行空格的意义，用户看起来就是一个空格。
		//从Unicode3.2开始，U+FEFF只能出现在字节流的开头，只能用于标识字节序，就如它的名称——字节序标记——所表示的一样；
		//除此以外的用法已被舍弃。取而代之的是，使用U+2060来表达零宽度无断空白。

		//在UTF-16中，字节顺序标记被放置为文件或字符串流的第一个字符，以标示在此文件或字符串流中，以所有十六比特为单位的字码的尾序（字节顺序）。
		//如果十六比特单位被表示成大尾序，这字节顺序标记字符在串行中将呈现0xFE，其后跟着0xFF（其中的0x用来标示十六进制）。
		//如果十六比特单位使用小尾序，这个字节串行为0xFF，其后接着0xFE。

		//TODO 不懂
		//而统一码中，值为U+FFFE的码位被保证将不会被指定成一个统一码字符。
		//这意味着0xFF、0xFE将只能被解释成小尾序中的U+FEFF（因为不可能是大尾序中的U+FFFE）。

		//UTF-8则没有字节顺序的议题。UTF-8编码过的字节顺序标记则被用来标示它是UTF-8的文件。
		//它只用来标示一个UTF-8的文件，而不用来说明字节顺序。
		//字节顺序标记在UTF-8中被表示为串行EF BB BF

		//虽然字节顺序标记亦可以用于UTF-32，但这个编码很少用于传输，其规则如同UTF-16。

		//对于已于IANA注册的字符集UTF-16BE、UTF-16LE、UTF-32BE和UTF-32LE等来说，不可使用字节顺序标记。
		//文档开头的U+FEFF会被解释成一个（已舍弃的）"零宽度无断空白"，因为这些字符集的名字已决定了其字节顺序。
		//对于已注册字符集UTF-16和UTF-32来说，一个开头的U+FEFF则用来表示字节顺序。

		//! The Unicode byte order mark.
		const u16 BOM = 0xFEFF;

		//! The size of the Unicode byte order mark in terms of the Unicode character size.
		const u8 BOM_UTF8_LEN = 3;
		const u8 BOM_UTF16_LEN = 1;
		const u8 BOM_UTF32_LEN = 1;

		//! Unicode byte order marks for file operations.
		const u8 BOM_ENCODE_UTF8[3] = { 0xEF, 0xBB, 0xBF };
		const u8 BOM_ENCODE_UTF16BE[2] = { 0xFE, 0xFF };
		const u8 BOM_ENCODE_UTF16LE[2] = { 0xFF, 0xFE };
		const u8 BOM_ENCODE_UTF32BE[4] = { 0x00, 0x00, 0xFE, 0xFF };
		const u8 BOM_ENCODE_UTF32LE[4] = { 0xFF, 0xFE, 0x00, 0x00 };

		//! The size in bytes of the Unicode byte marks for file operations.
		const u8 BOM_ENCODE_UTF8_LEN = 3;
		const u8 BOM_ENCODE_UTF16_LEN = 2;
		const u8 BOM_ENCODE_UTF32_LEN = 4;

		//! Returns the specified unicode byte order mark in a byte array.
		//! The byte order mark is the first few bytes in a text file that signifies its encoding.
		/** \param mode The Unicode encoding method that we want to get the byte order mark for.
		If EUTFE_UTF16 or EUTFE_UTF32 is passed, it uses the native system endianness. **/
		//! \return An array that contains a byte order mark.
		inline core::array<u8> getUnicodeBOM(ENUM_UTF_ENCODING mode)
		{
#define YON_USTRING_COPY_ARRAY(source, size) \
	memcpy(ret.pointer(), source, size); \
	ret.set_used(size)

			core::array<u8> ret(4);
			switch (mode)
			{
			case ENUM_UTF_ENCODING_UTF8:
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF8, BOM_ENCODE_UTF8_LEN);
				break;
			case ENUM_UTF_ENCODING_UTF16:
#ifdef __BIG_ENDIAN__
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF16BE, BOM_ENCODE_UTF16_LEN);
#else
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF16LE, BOM_ENCODE_UTF16_LEN);
#endif
				break;
			case ENUM_UTF_ENCODING_UTF16BE:
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF16BE, BOM_ENCODE_UTF16_LEN);
				break;
			case ENUM_UTF_ENCODING_UTF16LE:
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF16LE, BOM_ENCODE_UTF16_LEN);
				break;
			case ENUM_UTF_ENCODING_UTF32:
#ifdef __BIG_ENDIAN__
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF32BE, BOM_ENCODE_UTF32_LEN);
#else
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF32LE, BOM_ENCODE_UTF32_LEN);
#endif
				break;
			case ENUM_UTF_ENCODING_UTF32BE:
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF32BE, BOM_ENCODE_UTF32_LEN);
				break;
			case ENUM_UTF_ENCODING_UTF32LE:
				YON_USTRING_COPY_ARRAY(BOM_ENCODE_UTF32LE, BOM_ENCODE_UTF32_LEN);
				break;
			}
			return ret;

#undef YON_USTRING_COPY_ARRAY
		}

		//! Detects if the given data stream starts with a unicode BOM.
		//! \param data The data stream to check.
		//! \return The unicode BOM associated with the data stream, or ENUM_UTF_ENCODING_NONE if none was found.
		inline ENUM_UTF_ENCODING determineUnicodeBOM(const char* data)
		{
			if (memcmp(data, BOM_ENCODE_UTF8, 3) == 0) return ENUM_UTF_ENCODING_UTF8;
			if (memcmp(data, BOM_ENCODE_UTF16BE, 2) == 0) return ENUM_UTF_ENCODING_UTF16BE;
			if (memcmp(data, BOM_ENCODE_UTF16LE, 2) == 0) return ENUM_UTF_ENCODING_UTF16LE;
			if (memcmp(data, BOM_ENCODE_UTF32BE, 4) == 0) return ENUM_UTF_ENCODING_UTF32BE;
			if (memcmp(data, BOM_ENCODE_UTF32LE, 4) == 0) return ENUM_UTF_ENCODING_UTF32LE;
			return ENUM_UTF_ENCODING_NONE;
		}

	}

	

	//! UTF-16 string class.
	template <typename TAlloc = yonAllocatorAlign<uchar16_t> >
	class ustring16{
	public:

		///------------------///
		/// iterator classes ///
		///------------------///

		//! Access an element in a unicode string, allowing one to change it.
		class _ustring16_iterator_access
		{
		public:
			_ustring16_iterator_access(const ustring16<TAlloc>* s, size_t p) : ref(s), pos(p) {}

			//! Allow the class to be interpreted as a single UTF-32 character.
			operator uchar32_t() const
			{
				return _get();
			}

			//! Allow one to change the character in the unicode string.
			//! \param c The new character to use.
			//! \return Myself.
			_ustring16_iterator_access& operator=(const uchar32_t c)
			{
				_set(c);
				return *this;
			}

			//! Increments the value by 1.
			//! \return Myself.
			_ustring16_iterator_access& operator++()
			{
				_set(_get() + 1);
				return *this;
			}

			//! Increments the value by 1, returning the old value.
			//! \return A unicode character.
			uchar32_t operator++(int)
			{
				uchar32_t old = _get();
				_set(old + 1);
				return old;
			}

			//! Decrements the value by 1.
			//! \return Myself.
			_ustring16_iterator_access& operator--()
			{
				_set(_get() - 1);
				return *this;
			}

			//! Decrements the value by 1, returning the old value.
			//! \return A unicode character.
			uchar32_t operator--(int)
			{
				uchar32_t old = _get();
				_set(old - 1);
				return old;
			}

			//! Adds to the value by a specified amount.
			//! \param val The amount to add to this character.
			//! \return Myself.
			_ustring16_iterator_access& operator+=(int val)
			{
				_set(_get() + val);
				return *this;
			}

			//! Subtracts from the value by a specified amount.
			//! \param val The amount to subtract from this character.
			//! \return Myself.
			_ustring16_iterator_access& operator-=(int val)
			{
				_set(_get() - val);
				return *this;
			}

			//! Multiples the value by a specified amount.
			//! \param val The amount to multiply this character by.
			//! \return Myself.
			_ustring16_iterator_access& operator*=(int val)
			{
				_set(_get() * val);
				return *this;
			}

			//! Divides the value by a specified amount.
			//! \param val The amount to divide this character by.
			//! \return Myself.
			_ustring16_iterator_access& operator/=(int val)
			{
				_set(_get() / val);
				return *this;
			}

			//! Modulos the value by a specified amount.
			//! \param val The amount to modulo this character by.
			//! \return Myself.
			_ustring16_iterator_access& operator%=(int val)
			{
				_set(_get() % val);
				return *this;
			}

			//! Adds to the value by a specified amount.
			//! \param val The amount to add to this character.
			//! \return A unicode character.
			uchar32_t operator+(int val) const
			{
				return _get() + val;
			}

			//! Subtracts from the value by a specified amount.
			//! \param val The amount to subtract from this character.
			//! \return A unicode character.
			uchar32_t operator-(int val) const
			{
				return _get() - val;
			}

			//! Multiplies the value by a specified amount.
			//! \param val The amount to multiply this character by.
			//! \return A unicode character.
			uchar32_t operator*(int val) const
			{
				return _get() * val;
			}

			//! Divides the value by a specified amount.
			//! \param val The amount to divide this character by.
			//! \return A unicode character.
			uchar32_t operator/(int val) const
			{
				return _get() / val;
			}

			//! Modulos the value by a specified amount.
			//! \param val The amount to modulo this character by.
			//! \return A unicode character.
			uchar32_t operator%(int val) const
			{
				return _get() % val;
			}

		private:
			//! Gets a uchar32_t from our current position.
			uchar32_t _get() const
			{
				const uchar16_t* a = ref->c_str();
				if (!UTF16_IS_SURROGATE(a[pos]))
					return static_cast<uchar32_t>(a[pos]);
				else
				{
					if (pos + 1 >= ref->size_raw())
						return 0;

					return unicode::toUTF32(a[pos], a[pos + 1]);
				}
			}

			//! Sets a uchar32_t at our current position.
			void _set(uchar32_t c)
			{
				ustring16<TAlloc>* ref2 = const_cast<ustring16<TAlloc>*>(ref);
				const uchar16_t* a = ref2->c_str();
				//辅助平面(Supplementary Planes)中的码位，在UTF-16中被编码为一对16比特长的码元(即32bit,4Bytes)，称作代理对(surrogate pair)
				//高位的10比特的值(值的范围为0..0x3FF)被加上0xD800得到第一个码元或称作高位代理(high surrogate), 值的范围是0xD800..0xDBFF. 
				//低位的10比特的值(值的范围也是0..0x3FF)被加上0xDC00得到第二个码元或称作低位代理(low surrogate), 现在值的范围是0xDC00..0xDFFF. 
				if (c > 0xFFFF)
				{
					// c will be multibyte, so split it up into the high and low surrogate pairs.
					//refer to:http://www.linuxidc.com/Linux/2011-05/35485.htm
					//码位减去0x10000, 得到的值的范围为20比特长的0..0xFFFFF.
					//相当于x=c-0x0010000UL;
					uchar16_t x = static_cast<uchar16_t>(c);
					//相当于0xD800|(取20比特位中头5位<<6)|(取c的低16位>>10)
					uchar16_t vh = UTF16_HI_SURROGATE | ((((c >> 16) & ((1 << 5) - 1)) - 1) << 6) | (x >> 10);
					//相当于0xDC00|(x&0x03FF)
					uchar16_t vl = UTF16_LO_SURROGATE | (x & ((1 << 10) - 1));

					//TODO 待理解
					// If the previous position was a surrogate pair, just replace them.  Else, insert the low pair.
					if (UTF16_IS_SURROGATE_HI(a[pos]) && pos + 1 != ref2->size_raw())
						ref2->replace_raw(vl, pos + 1);
					else ref2->insert_raw(vl, pos + 1);

					ref2->replace_raw(vh, pos);
				}
				else
				{
					// c will be a single byte.
					uchar16_t vh = static_cast<uchar16_t>(c);

					// If the previous position was a surrogate pair, remove the extra byte.
					if (UTF16_IS_SURROGATE_HI(a[pos]))
						ref2->erase_raw(pos + 1);

					ref2->replace_raw(vh, pos);
				}
			}

			const ustring16<TAlloc>* ref;
			size_t pos;
		};
		typedef typename ustring16<TAlloc>::_ustring16_iterator_access access;

		class _ustring16_const_iterator{
		public:
			//typedef _ustring16_const_iterator _Iter;
			//typedef std::iterator<std::bidirectional_iterator_tag, access, ptrdiff_t, const access, const access> _Base;
			typedef const access const_pointer;
			typedef const access const_reference;

			typedef access value_type;
			typedef size_t difference_type;
			typedef size_t distance_type;
			typedef const_pointer pointer;
			typedef const_reference reference;

			//! Constructors.
			_ustring16_const_iterator(const _ustring16_const_iterator& i) : ref(i.ref), pos(i.pos) {}
			_ustring16_const_iterator(const ustring16<TAlloc>& s) : ref(&s), pos(0) {}
			_ustring16_const_iterator(const ustring16<TAlloc>& s, const size_t p) : ref(&s), pos(0)
			{
				if (ref->size_raw() == 0 || p == 0)
					return;

				// Go to the appropriate position.
				size_t i = p;
				size_t sr = ref->size_raw();
				const uchar16_t* a = ref->c_str();
				while (i != 0 && pos < sr)
				{
					if (UTF16_IS_SURROGATE_HI(a[pos]))
						pos += 2;
					else ++pos;
					--i;
				}
			}

			//! Test for equalness.
			bool operator==(const _ustring16_const_iterator& iter) const
			{
				if (ref == iter.ref && pos == iter.pos)
					return true;
				return false;
			}

			//! Test for unequalness.
			bool operator!=(const _ustring16_const_iterator& iter) const
			{
				//if (ref != iter.ref || pos != iter.pos)
				//	return true;
				//return false;
				return !(iter==*this);
			}

			//TODO 索引报警
			//! Switch to the next full character in the string.
			_ustring16_const_iterator& operator++()
			{	// ++iterator
				if (pos == ref->size_raw()) return *this;
				const uchar16_t* a = ref->c_str();
				if (UTF16_IS_SURROGATE_HI(a[pos]))
					pos += 2;			// TODO: check for valid low surrogate?
				else ++pos;
				if (pos > ref->size_raw()) pos = ref->size_raw();
				return *this;
			}

			//! Switch to the next full character in the string, returning the previous position.
			_ustring16_const_iterator operator++(int)
			{	// iterator++
				_ustring16_const_iterator _tmp(*this);
				++*this;
				return _tmp;
			}

			//TODO 索引报警
			//! Switch to the previous full character in the string.
			_ustring16_const_iterator& operator--()
			{	// --iterator
				if (pos == 0) return *this;
				const uchar16_t* a = ref->c_str();
				--pos;
				if (UTF16_IS_SURROGATE_LO(a[pos]) && pos != 0)	// low surrogate, go back one more.
					--pos;
				return *this;
			}

			//! Switch to the previous full character in the string, returning the previous position.
			_ustring16_const_iterator operator--(int)
			{	// iterator--
				_ustring16_const_iterator _tmp(*this);
				--*this;
				return _tmp;
			}

			//TODO 索引报警
			//! Advance a specified number of full characters in the string.
			//! \return Myself.
			_ustring16_const_iterator& operator+=(const difference_type v)
			{
				if (v == 0) return *this;
				if (v < 0) return operator-=(v * -1);

				if (pos >= ref->size_raw())
					return *this;

				// Go to the appropriate position.
				difference_type i = v;
				size_t sr = ref->size_raw();
				const uchar16_t* a = ref->c_str();
				while (i != 0 && pos < sr)
				{
					if (UTF16_IS_SURROGATE_HI(a[pos]))
						pos += 2;
					else ++pos;
					--i;
				}
				if (pos > sr)
					pos = sr;

				return *this;
			}

			//TODO 索引报警
			//! Go back a specified number of full characters in the string.
			//! \return Myself.
			_ustring16_const_iterator& operator-=(const difference_type v)
			{
				if (v == 0) return *this;
				if (v > 0) return operator+=(v * -1);

				if (pos == 0)
					return *this;

				// Go to the appropriate position.
				difference_type i = v;
				const uchar16_t* a = ref->c_str();
				while (i != 0 && pos != 0)
				{
					--pos;
					if (UTF16_IS_SURROGATE_LO(a[pos]) != 0 && pos != 0)
						--pos;
					--i;
				}

				return *this;
			}

			//! Return a new iterator that is a variable number of full characters forward from the current position.
			_ustring16_const_iterator operator+(const difference_type v) const
			{
				_ustring16_const_iterator ret(*this);
				ret += v;
				return ret;
			}

			//! Return a new iterator that is a variable number of full characters backward from the current position.
			_ustring16_const_iterator operator-(const difference_type v) const
			{
				_ustring16_const_iterator ret(*this);
				ret -= v;
				return ret;
			}

			//TODO 报警
			//! Returns the distance between two iterators.
			difference_type operator-(const _ustring16_const_iterator& iter) const
			{
				// Make sure we reference the same object!
				if (ref != iter.ref)
					return difference_type();

				_ustring16_const_iterator i = iter;
				difference_type ret;

				// Walk up.
				if (pos > i.pos)
				{
					while (pos > i.pos)
					{
						++i;
						++ret;
					}
					return ret;
				}

				// Walk down.
				while (pos < i.pos)
				{
					--i;
					--ret;
				}
				return ret;
			}

			//! Accesses the full character at the iterator's position.
			const_reference operator*() const
			{
				if (pos >= ref->size_raw())
				{
					const uchar16_t* a = ref->c_str();
					size_t p = ref->size_raw();
					if (UTF16_IS_SURROGATE_LO(a[p]))
						--p;
					reference ret(ref, p);
					return ret;
				}
				const_reference ret(ref, pos);
				return ret;
			}

			//! Accesses the full character at the iterator's position.
			reference operator*()
			{
				if (pos >= ref->size_raw())
				{
					const uchar16_t* a = ref->c_str();
					size_t p = ref->size_raw();
					if (UTF16_IS_SURROGATE_LO(a[p]))
						--p;
					reference ret(ref, p);
					return ret;
				}
				reference ret(ref, pos);
				return ret;
			}

			//! Accesses the full character at the iterator's position.
			const_pointer operator->() const
			{
				return operator*();
			}

			//! Accesses the full character at the iterator's position.
			pointer operator->()
			{
				return operator*();
			}

			//! Is the iterator at the start of the string?
			bool atStart() const
			{
				return pos == 0;
			}

			//! Is the iterator at the end of the string?
			bool atEnd() const
			{
				const uchar16_t* a = ref->c_str();
				if (UTF16_IS_SURROGATE(a[pos]))
					return (pos + 1) >= ref->size_raw();
				else return pos >= ref->size_raw();
			}

			//! Moves the iterator to the start of the string.
			void toStart()
			{
				pos = 0;
			}

			//! Moves the iterator to the end of the string.
			void toEnd()
			{
				const uchar16_t* a = ref->c_str();
				pos = ref->size_raw();
			}

			//! Returns the iterator's position.
			//! \return The iterator's position.
			size_t getPos() const
			{
				return pos;
			}

		protected:
			const ustring16<TAlloc>* ref;
			size_t pos;
		};

		//! Iterator to iterate through a UTF-16 string.
		class _ustring16_iterator : public _ustring16_const_iterator
		{
		public:
			//typedef _ustring16_iterator _Iter;
			typedef _ustring16_const_iterator _Base;
			typedef typename _Base::const_pointer const_pointer;
			typedef typename _Base::const_reference const_reference;

			typedef typename _Base::value_type value_type;
			typedef typename _Base::difference_type difference_type;
			typedef typename _Base::distance_type distance_type;
			typedef access pointer;
			typedef access reference;

			using _Base::pos;
			using _Base::ref;

			//! Constructors.
			_ustring16_iterator(const _ustring16_iterator& i) : _ustring16_const_iterator(i) {}
			_ustring16_iterator(const ustring16<TAlloc>& s) : _ustring16_const_iterator(s) {}
			_ustring16_iterator(const ustring16<TAlloc>& s, const size_t p) : _ustring16_const_iterator(s, p) {}

			//! Accesses the full character at the iterator's position.
			reference operator*() const
			{
				if (pos >= ref->size_raw())
				{
					const uchar16_t* a = ref->c_str();
					size_t p = ref->size_raw();
					if (UTF16_IS_SURROGATE_LO(a[p]))
						--p;
					reference ret(ref, p);
					return ret;
				}
				reference ret(ref, pos);
				return ret;
			}

			//! Accesses the full character at the iterator's position.
			reference operator*()
			{
				if (pos >= ref->size_raw())
				{
					const uchar16_t* a = ref->c_str();
					size_t p = ref->size_raw();
					if (UTF16_IS_SURROGATE_LO(a[p]))
						--p;
					reference ret(ref, p);
					return ret;
				}
				reference ret(ref, pos);
				return ret;
			}

			//! Accesses the full character at the iterator's position.
			pointer operator->() const
			{
				return operator*();
			}

			//! Accesses the full character at the iterator's position.
			pointer operator->()
			{
				return operator*();
			}
		};

		typedef typename ustring16<TAlloc>::_ustring16_iterator iterator;
		typedef typename ustring16<TAlloc>::_ustring16_const_iterator const_iterator;

		///----------------------///
		/// end iterator classes ///
		///----------------------///


		///------------------///
		/// static variables ///
		///------------------///
		static const size_t npos = -1;


		//! Default constructor
		ustring16()
			: elements(0), allocated(1), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif
			elements = allocator.allocate(1); // new u16[1];
			elements[0] = 0x0;
		}

		//! Constructor
		ustring16(const ustring16<TAlloc>& other)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif
			*this = other;
		}


		//! Constructor from other string types
		template <class B, class A>
		ustring16(const string<B, A>& other)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif
			*this = other;
		}

#ifndef USTRING_CPP0X_NEWLITERALS
		//! Constructor for copying a character string from a pointer.
		ustring16(const char* const c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			build(c, strlen(c));
			//append((uchar8_t*)c);
		}


		//! Constructor for copying a character string from a pointer with a given length.
		ustring16(const char* const c, size_t length)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			build(c, length);
		}
#endif

		//! Constructor for copying a UTF-8 string from a pointer.
		ustring16(const uchar8_t* const c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c);
		}


		//! Constructor for copying a UTF-8 string from a single char.
		ustring16(const char c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append((uchar32_t)c);
		}


		//! Constructor for copying a UTF-8 string from a pointer with a given length.
		ustring16(const uchar8_t* const c, size_t length)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c, length);
		}


		//! Constructor for copying a UTF-16 string from a pointer.
		ustring16(const uchar16_t* const c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c);
		}


		//! Constructor for copying a UTF-16 string from a pointer with a given length
		ustring16(const uchar16_t* const c, size_t length)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c, length);
		}


		//! Constructor for copying a UTF-32 string from a pointer.
		ustring16(const uchar32_t* const c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c);
		}


		//! Constructor for copying a UTF-32 from a pointer with a given length.
		ustring16(const uchar32_t* const c, size_t length)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			append(c, length);
		}


		//! Constructor for copying a wchar_t string from a pointer.
		ustring16(const wchar_t* const c)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			if (sizeof(wchar_t) == 4)
				append(reinterpret_cast<const uchar32_t* const>(c));
			else if (sizeof(wchar_t) == 2)
				append(reinterpret_cast<const uchar16_t* const>(c));
			else if (sizeof(wchar_t) == 1)
				append(reinterpret_cast<const uchar8_t* const>(c));
		}


		//! Constructor for copying a wchar_t string from a pointer with a given length.
		ustring16(const wchar_t* const c, size_t length)
			: elements(0), allocated(0), used(0)
		{
#if __BIG_ENDIAN__
			encoding = ENUM_UTF_ENCODING_UTF16BE;
#else
			encoding = ENUM_UTF_ENCODING_UTF16LE;
#endif

			if (sizeof(wchar_t) == 4)
				append(reinterpret_cast<const uchar32_t* const>(c), length);
			else if (sizeof(wchar_t) == 2)
				append(reinterpret_cast<const uchar16_t* const>(c), length);
			else if (sizeof(wchar_t) == 1)
				append(reinterpret_cast<const uchar8_t* const>(c), length);
		}


#ifdef USTRING_CPP0X
		//! Constructor for moving a ustring16
		ustring16(ustring16<TAlloc>&& other)
			: elements(other.elements), encoding(other.encoding), allocated(other.allocated), used(other.used)
		{
			other.elements = 0;
			other.allocated = 0;
			other.used = 0;
		}
#endif


		//! Destructor
		~ustring16()
		{
			allocator.deallocate(elements); // delete [] elements;
		}

		//! Assignment operator
		ustring16& operator=(const ustring16<TAlloc>& other)
		{
			if (this == &other)
				return *this;

			used = other.size_raw();
			if (used >= allocated)
			{
				allocator.deallocate(elements); // delete [] elements;
				allocated = used + 1;
				elements = allocator.allocate(used + 1); //new u16[used];
			}

			const uchar16_t* p = other.c_str();
			for (size_t i=0; i<=used; ++i, ++p)
				elements[i] = *p;

			elements[used] = 0;

			// Validate our new UTF-16 string.
			validate();

			return *this;
		}


#ifdef USTRING_CPP0X
		//! Move assignment operator
		ustring16& operator=(ustring16<TAlloc>&& other)
		{
			if (this != &other)
			{
				//std::cout << "MOVE operator=" << std::endl;
				allocator.deallocate(elements);

				elements = other.elements;
				allocated = other.allocated;
				encoding = other.encoding;
				used = other.used;
				other.elements = 0;
				other.used = 0;
			}
			return *this;
		}
#endif


		//! Assignment operator for other string types
		template <class B, class A>
		ustring16<TAlloc>& operator=(const string<B, A>& other)
		{
			*this = other.c_str();
			return *this;
		}


		//! Assignment operator for UTF-8 strings
		ustring16<TAlloc>& operator=(const uchar8_t* const c)
		{
			if (!elements)
			{
				elements = allocator.allocate(1); //new u16[1];
				allocated = 1;
			}
			used = 0;
			elements[used] = 0x0;
			if (!c) return *this;

			//! Append our string now.
			append(c);
			return *this;
		}


		//! Assignment operator for UTF-16 strings
		ustring16<TAlloc>& operator=(const uchar16_t* const c)
		{
			if (!elements)
			{
				elements = allocator.allocate(1); //new u16[1];
				allocated = 1;
			}
			used = 0;
			elements[used] = 0x0;
			if (!c) return *this;

			//! Append our string now.
			append(c);
			return *this;
		}


		//! Assignment operator for UTF-32 strings
		ustring16<TAlloc>& operator=(const uchar32_t* const c)
		{
			if (!elements)
			{
				elements = allocator.allocate(1); //new u16[1];
				allocated = 1;
			}
			used = 0;
			elements[used] = 0x0;
			if (!c) return *this;

			//! Append our string now.
			append(c);
			return *this;
		}


		//! Assignment operator for wchar_t strings.
		/** Note that this assumes that a correct unicode string is stored in the wchar_t string.
		Since wchar_t changes depending on its platform, it could either be a UTF-8, -16, or -32 string.
		This function assumes you are storing the correct unicode encoding inside the wchar_t string. **/
		ustring16<TAlloc>& operator=(const wchar_t* const c)
		{
			if (sizeof(wchar_t) == 4)
				*this = reinterpret_cast<const uchar32_t* const>(c);
			else if (sizeof(wchar_t) == 2)
				*this = reinterpret_cast<const uchar16_t* const>(c);
			else if (sizeof(wchar_t) == 1)
				*this = reinterpret_cast<const uchar8_t* const>(c);

			return *this;
		}


		//! Assignment operator for other strings.
		/** Note that this assumes that a correct unicode string is stored in the string. **/
		template <class B>
		ustring16<TAlloc>& operator=(const B* const c)
		{
			if (sizeof(B) == 4)
				*this = reinterpret_cast<const uchar32_t* const>(c);
			else if (sizeof(B) == 2)
				*this = reinterpret_cast<const uchar16_t* const>(c);
			else if (sizeof(B) == 1)
				*this = reinterpret_cast<const uchar8_t* const>(c);

			return *this;
		}


		//! Direct access operator
		access operator [](const size_t index)
		{
			YON_DEBUG_BREAK_IF(index>=size()); // bad index
				iterator iter(*this, index);
			return iter.operator*();
		}


		//! Direct access operator
		const access operator [](const size_t index) const
		{
			YON_DEBUG_BREAK_IF(index>=size()); // bad index
				const_iterator iter(*this, index);
			return iter.operator*();
		}


		//! Equality operator
		bool operator ==(const uchar16_t* const str) const
		{
			if (!str)
				return false;

			size_t i;
			for (i=0; elements[i] && str[i]; ++i)
				if (elements[i] != str[i])
					return false;

			return !elements[i] && !str[i];
		}


		//! Equality operator
		bool operator ==(const ustring16<TAlloc>& other) const
		{
			for (size_t i=0; elements[i] && other.elements[i]; ++i)
				if (elements[i] != other.elements[i])
					return false;

			return used == other.used;
		}


		//! Is smaller comparator
		bool operator <(const ustring16<TAlloc>& other) const
		{
			for (size_t i=0; elements[i] && other.elements[i]; ++i)
			{
				if (elements[i] == other.elements[i])
					continue;
				return elements[i] < other.elements[i];
			}

			return used < other.used;
		}


		//! Inequality operator
		bool operator !=(const uchar16_t* const str) const
		{
			return !(*this == str);
		}


		//! Inequality operator
		bool operator !=(const ustring16<TAlloc>& other) const
		{
			return !(*this == other);
		}

		//! Appends a character to this ustring16.
		//! \param c Character to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& operator += (char c)
		{
			append((uchar32_t)c);
			return *this;
		}


		//! Appends a character to this ustring16.
		//! \param c Character to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& operator += (uchar32_t c)
		{
			append(c);
			return *this;
		}


		//! Appends a char ustring16 to this ustring16.
		//! \param c Char ustring16 to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& operator += (const uchar16_t* const c)
		{
			append(c);
			return *this;
		}


		//! Appends a ustring16 to this ustring16.
		//! \param other ustring16 to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& operator += (const ustring16<TAlloc>& other)
		{
			append(other);
			return *this;
		}

		//TODO 使用成员变量记录
		//! Returns the length of a ustring16 in full characters.
		//! \return Length of a ustring16 in full characters.
		size_t size() const
		{
			const_iterator i(*this, 0);
			size_t pos = 0;
			while (!i.atEnd())
			{
				++i;
				++pos;
			}
			return pos;
		}


		//! Informs if the ustring is empty or not.
		//! \return True if the ustring is empty, false if not.
		bool empty() const
		{
			return (size_raw() == 0);
		}


		//! Returns a pointer to the raw UTF-16 string data.
		//! \return pointer to C-style NUL terminated elements of UTF-16 code points.
		const uchar16_t* c_str() const
		{
			return elements;
		}


		//! Compares the first n characters of this string with another.
		//! \param other Other string to compare to.
		//! \param n Number of characters to compare.
		//! \return True if the n first characters of both strings are equal.
		bool equalsn(const ustring16<TAlloc>& other, size_t n) const
		{
			size_t i;
			const uchar16_t* oa = other.c_str();
			for(i=0; elements[i] && oa[i] && i < n; ++i)
				if (elements[i] != oa[i])
					return false;

			// if one (or both) of the strings was smaller then they
			// are only equal if they have the same length
			return (i == n) || (used == other.used);
		}


		//! Compares the first n characters of this string with another.
		//! \param str Other string to compare to.
		//! \param n Number of characters to compare.
		//! \return True if the n first characters of both strings are equal.
		bool equalsn(const uchar16_t* const str, size_t n) const
		{
			if (!str)
				return false;
			size_t i;
			for(i=0; elements[i] && str[i] && i < n; ++i)
				if (elements[i] != str[i])
					return false;

			// if one (or both) of the strings was smaller then they
			// are only equal if they have the same length
			return (i == n) || (elements[i] == 0 && str[i] == 0);
		}


		//! Appends a character to this ustring16
		//! \param character The character to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(uchar32_t character)
		{
			if (used + 2 >= allocated)
				reallocate(used + 2);

			if (character > 0xFFFF)
			{
				used += 2;

				// character will be multibyte, so split it up into a surrogate pair.
				uchar16_t x = static_cast<uchar16_t>(character);
				uchar16_t vh = UTF16_HI_SURROGATE | ((((character >> 16) & ((1 << 5) - 1)) - 1) << 6) | (x >> 10);
				uchar16_t vl = UTF16_LO_SURROGATE | (x & ((1 << 10) - 1));
				elements[used-2] = vh;
				elements[used-1] = vl;
			}
			else
			{
				++used;
				elements[used-1] = character;
			}
			elements[used] = 0x0;

			return *this;
		}


		//TODO 待测试
		//! Appends a UTF-8 string to this ustring16
		//! \param other The UTF-8 string to append.
		//! \param length The length of the string to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(const uchar8_t* const other, size_t length=0xffffffff)
		{
			if (!other)
				return *this;

			// Determine if the string is long enough for a BOM.
			size_t len = 0;
			const uchar8_t* p = other;
			do
			{
				++len;
			} while (*p++ && len < unicode::BOM_ENCODE_UTF8_LEN);

			// Check for BOM.
			ENUM_UTF_ENCODING c_bom = ENUM_UTF_ENCODING_NONE;
			if (len == unicode::BOM_ENCODE_UTF8_LEN)
			{
				if (memcmp(other, unicode::BOM_ENCODE_UTF8, unicode::BOM_ENCODE_UTF8_LEN) == 0)
					c_bom = ENUM_UTF_ENCODING_UTF8;
			}

			// If a BOM was found, don't include it in the string.
			const uchar8_t* c2 = other;
			if (c_bom != ENUM_UTF_ENCODING_NONE)
			{
				c2 = other + unicode::BOM_UTF8_LEN;
				length -= unicode::BOM_UTF8_LEN;
			}

			// Calculate the size of the string to read in.
			len = 0;
			p = c2;
			do
			{
				++len;
			} while(*p++ && len < length);
			if (len > length)
				len = length;

			// If we need to grow the elements, do it now.
			if (used + len >= allocated)
				reallocate(used + (len * 2));
			size_t start = used;

			// Convert UTF-8 to UTF-16.
			size_t pos = start;
			for (size_t l = 0; l<len;)
			{
				++used;
				if (((c2[l] >> 6) & 0x03) == 0x02)
				{	// Invalid continuation byte.
					elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
					++l;
				}
				else if (c2[l] == 0xC0 || c2[l] == 0xC1)
				{	// Invalid byte - overlong encoding.
					elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
					++l;
				}
				else if ((c2[l] & 0xF8) == 0xF0)
				{	// 4 bytes UTF-8, 2 bytes UTF-16.
					// Check for a full string.
					if ((l + 3) >= len)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						l += 3;
						break;
					}

					// Validate.
					bool valid = true;
					u8 l2 = 0;
					if (valid && (((c2[l+1] >> 6) & 0x03) == 0x02)) ++l2; else valid = false;
					if (valid && (((c2[l+2] >> 6) & 0x03) == 0x02)) ++l2; else valid = false;
					if (valid && (((c2[l+3] >> 6) & 0x03) == 0x02)) ++l2; else valid = false;
					if (!valid)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						l += l2;
						continue;
					}

					// Decode.
					uchar8_t b1 = ((c2[l] & 0x7) << 2) | ((c2[l+1] >> 4) & 0x3);
					uchar8_t b2 = ((c2[l+1] & 0xF) << 4) | ((c2[l+2] >> 2) & 0xF);
					uchar8_t b3 = ((c2[l+2] & 0x3) << 6) | (c2[l+3] & 0x3F);
					uchar32_t v = b3 | ((uchar32_t)b2 << 8) | ((uchar32_t)b1 << 16);

					// Split v up into a surrogate pair.
					uchar16_t x = static_cast<uchar16_t>(v);
					uchar16_t vh = UTF16_HI_SURROGATE | ((((v >> 16) & ((1 << 5) - 1)) - 1) << 6) | (x >> 10);
					uchar16_t vl = UTF16_LO_SURROGATE | (x & ((1 << 10) - 1));

					elements[pos++] = vh;
					elements[pos++] = vl;
					l += 4;
					++used;		// Using two shorts this time, so increase used by 1.
				}
				else if ((c2[l] & 0xF0) == 0xE0)
				{	// 3 bytes UTF-8, 1 byte UTF-16.
					// Check for a full string.
					if ((l + 2) >= len)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						l += 2;
						break;
					}

					// Validate.
					bool valid = true;
					u8 l2 = 0;
					if (valid && (((c2[l+1] >> 6) & 0x03) == 0x02)) ++l2; else valid = false;
					if (valid && (((c2[l+2] >> 6) & 0x03) == 0x02)) ++l2; else valid = false;
					if (!valid)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						l += l2;
						continue;
					}

					// Decode.
					uchar8_t b1 = ((c2[l] & 0xF) << 4) | ((c2[l+1] >> 2) & 0xF);
					uchar8_t b2 = ((c2[l+1] & 0x3) << 6) | (c2[l+2] & 0x3F);
					uchar16_t ch = b2 | ((uchar16_t)b1 << 8);
					elements[pos++] = ch;
					l += 3;
				}
				else if ((c2[l] & 0xE0) == 0xC0)
				{	// 2 bytes UTF-8, 1 byte UTF-16.
					// Check for a full string.
					if ((l + 1) >= len)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						l += 1;
						break;
					}

					// Validate.
					if (((c2[l+1] >> 6) & 0x03) != 0x02)
					{
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
						++l;
						continue;
					}

					// Decode.
					uchar8_t b1 = (c2[l] >> 2) & 0x7;
					uchar8_t b2 = ((c2[l] & 0x3) << 6) | (c2[l+1] & 0x3F);
					uchar16_t ch = b2 | ((uchar16_t)b1 << 8);
					elements[pos++] = ch;
					l += 2;
				}
				else
				{	// 1 byte UTF-8, 1 byte UTF-16.
					// Validate.
					if (c2[l] > 0x7F)
					{	// Values above 0xF4 are restricted and aren't used.  By now, anything above 0x7F is invalid.
						elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
					}
					else elements[pos++] = static_cast<uchar16_t>(c2[l]);
					++l;
				}
			}
			elements[used] = 0;

			// Validate our new UTF-16 string.
			validate();

			return *this;
		}


		//TODO 待测试
		//! Appends a UTF-16 string to this ustring16
		//! \param other The UTF-16 string to append.
		//! \param length The length of the string to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(const uchar16_t* const other, size_t length=0xffffffff)
		{
			if (!other)
				return *this;

			// Determine if the string is long enough for a BOM.
			size_t len = 0;
			const uchar16_t* p = other;
			do
			{
				++len;
			} while (*p++ && len < unicode::BOM_ENCODE_UTF16_LEN);

			// Check for the BOM to determine the string's endianness.
			ENUM_UTF_ENDIAN c_end = ENUM_UTF_ENDIAN_NATIVE;
			if (memcmp(other, unicode::BOM_ENCODE_UTF16LE, unicode::BOM_ENCODE_UTF16_LEN) == 0)
				c_end = ENUM_UTF_ENDIAN_LITTLE;
			else if (memcmp(other, unicode::BOM_ENCODE_UTF16BE, unicode::BOM_ENCODE_UTF16_LEN) == 0)
				c_end = ENUM_UTF_ENDIAN_BIG;

			// If a BOM was found, don't include it in the string.
			const uchar16_t* c2 = other;
			if (c_end != ENUM_UTF_ENDIAN_NATIVE)
			{
				c2 = other + unicode::BOM_UTF16_LEN;
				length -= unicode::BOM_UTF16_LEN;
			}

			// Calculate the size of the string to read in.
			len = 0;
			p = c2;
			do
			{
				++len;
			} while(*p++ && len < length);
			if (len > length)
				len = length;

			// If we need to grow the size of the elements, do it now.
			if (used + len >= allocated)
				reallocate(used + (len * 2));
			size_t start = used;
			used += len;

			// Copy the string now.
			ENUM_UTF_ENDIAN m_end = getEndianness();
			for (size_t l = start; l < start + len; ++l)
			{
				elements[l] = (uchar16_t)c2[l];
				if (c_end != ENUM_UTF_ENDIAN_NATIVE && c_end != m_end)
					elements[l] = unicode::swapEndian16(elements[l]);
			}

			elements[used] = 0;

			// Validate our new UTF-16 string.
			validate();
			return *this;
		}


		//! Appends a UTF-32 string to this ustring16
		//! \param other The UTF-32 string to append.
		//! \param length The length of the string to append.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(const uchar32_t* const other, size_t length=0xffffffff)
		{
			if (!other)
				return *this;

			// Check for the BOM to determine the string's endianness.
			ENUM_UTF_ENDIAN c_end = ENUM_UTF_ENDIAN_NATIVE;
			if (memcmp(other, unicode::BOM_ENCODE_UTF32LE, unicode::BOM_ENCODE_UTF32_LEN) == 0)
				c_end = ENUM_UTF_ENDIAN_LITTLE;
			else if (memcmp(other, unicode::BOM_ENCODE_UTF32BE, unicode::BOM_ENCODE_UTF32_LEN) == 0)
				c_end = ENUM_UTF_ENDIAN_BIG;

			// If a BOM was found, don't include it in the string.
			const uchar32_t* c2 = other;
			if (c_end != ENUM_UTF_ENDIAN_NATIVE)
			{
				c2 = other + unicode::BOM_UTF32_LEN;
				length -= unicode::BOM_UTF32_LEN;
			}

			// Calculate the size of the string to read in.
			size_t len = 0;
			const uchar32_t* p = c2;
			do
			{
				++len;
			} while(*p++ && len < length);
			if (len > length)
				len = length;

			// If we need to grow the size of the elements, do it now.
			// In case all of the UTF-32 string is split into surrogate pairs, do len * 2.
			if (used + (len * 2) >= allocated)
				reallocate(used + ((len * 2) * 2));
			size_t start = used;

			// Convert UTF-32 to UTF-16.
			ENUM_UTF_ENDIAN m_end = getEndianness();
			size_t pos = start;
			for (size_t l = 0; l<len; ++l)
			{
				++used;

				uchar32_t ch = c2[l];
				if (c_end != ENUM_UTF_ENDIAN_NATIVE && c_end != m_end)
					ch = unicode::swapEndian32(ch);

				if (ch > 0xFFFF)
				{
					// Split ch up into a surrogate pair as it is over 16 bits long.
					uchar16_t x = static_cast<uchar16_t>(ch);
					uchar16_t vh = UTF16_HI_SURROGATE | ((((ch >> 16) & ((1 << 5) - 1)) - 1) << 6) | (x >> 10);
					uchar16_t vl = UTF16_LO_SURROGATE | (x & ((1 << 10) - 1));
					elements[pos++] = vh;
					elements[pos++] = vl;
					++used;		// Using two shorts, so increased used again.
				}
				else if (ch >= 0xD800 && ch <= 0xDFFF)
				{
					// Between possible UTF-16 surrogates (invalid!)
					elements[pos++] = unicode::UTF_REPLACEMENT_CHARACTER;
				}
				else elements[pos++] = static_cast<uchar16_t>(ch);
			}
			elements[used] = 0;

			// Validate our new UTF-16 string.
			validate();

			return *this;
		}


		//! Appends a ustring16 to this ustring16
		//! \param other The string to append to this one.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(const ustring16<TAlloc>& other)
		{
			const uchar16_t* oa = other.c_str();

			size_t len = other.size_raw();

			if (used + len >= allocated)
				reallocate(used + len);

			for (size_t l=0; l<len; ++l)
				elements[used+l] = oa[l];

			used += len;
			elements[used] = 0;

			return *this;
		}


		//! Appends a certain amount of characters of a ustring16 to this ustring16.
		//! \param other The string to append to this one.
		//! \param length How many characters of the other string to add to this one.
		//! \return A reference to our current string.
		ustring16<TAlloc>& append(const ustring16<TAlloc>& other, size_t length)
		{
			if (other.size() == 0)
				return *this;

			if (other.size() < length)
			{
				append(other);
				return *this;
			}

			if (used + length * 2 >= allocated)
				reallocate(used + length * 2);

			const_iterator iter(other, 0);
			size_t l = length;
			while (!iter.atEnd() && l)
			{
				uchar32_t c = *iter;
				append(c);
				++iter;
				--l;
			}

			return *this;
		}


		//! Reserves some memory.
		//! \param count The amount of characters to reserve.
		void reserve(size_t count)
		{
			if (count < allocated)
				return;

			reallocate(count);
		}


		//! Finds first occurrence of character.
		//! \param c The character to search for.
		//! \return Position where the character has been found, or ustring::npos if not found.
		size_t findFirst(uchar32_t c) const
		{
			const_iterator i(*this, 0);

			size_t pos = 0;
			while (!i.atEnd())
			{
				uchar32_t t = *i;
				if (c == t)
					return pos;
				++pos;
				++i;
			}

			return ustring16<TAlloc>::npos;
		}

		//! Finds first occurrence of a character of a list.
		//! \param c A list of characters to find. For example if the method should find the first occurrence of 'a' or 'b', this parameter should be "ab".
		//! \param count The amount of characters in the list. Usually, this should be strlen(c).
		//! \return Position where one of the characters has been found, or ustring::npos if not found.
		size_t findFirstChar(const uchar32_t* const c, size_t count=1) const
		{
			if (!c || !count)
				return ustring16<TAlloc>::npos;

			const_iterator i(*this, 0);

			size_t pos = 0;
			while (!i.atEnd())
			{
				uchar32_t t = *i;
				for (size_t j = 0; j < count; ++j)
					if (t == c[j])
						return pos;
				++pos;
				++i;
			}

			return ustring16<TAlloc>::npos;
		}


		//! Finds first position of a character not in a given list.
		//! \param c A list of characters to NOT find. For example if the method should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
		//! \param count The amount of characters in the list. Usually, this should be strlen(c).
		//! \return Position where the character has been found, or ustring::npos if not found.
		size_t findFirstCharNotInList(const uchar32_t* const c, size_t count=1) const
		{
			if (!c || !count)
				return ustring16<TAlloc>::npos;

			const_iterator i(*this, 0);

			size_t pos = 0;
			while (!i.atEnd())
			{
				uchar32_t t = *i;
				size_t j;
				for (j = 0; j < count; ++j)
					if (t == c[j])
						break;

				if (j == count)
					return pos;
				++pos;
				++i;
			}

			return ustring16<TAlloc>::npos;
		}

		//! Finds last position of a character not in a given list.
		//! \param c A list of characters to NOT find. For example if the method should find the first occurrence of a character not 'a' or 'b', this parameter should be "ab".
		//! \param count The amount of characters in the list. Usually, this should be strlen(c).
		//! \return Position where the character has been found, or ustring::npos if not found.
		size_t findLastCharNotInList(const uchar32_t* const c, size_t count=1) const
		{
			if (!c || !count)
				return ustring16<TAlloc>::npos;

			const_iterator i(end());
			--i;

			size_t pos = size();
			if (pos == 0)
				return ustring16<TAlloc>::npos;
			--pos;

			while (!i.atStart())
			{
				uchar32_t t = *i;
				size_t j;
				for (j = 0; j < count; ++j)
					if (t == c[j])
						break;

				if (j == count)
					return pos;
				--pos;
				--i;
			}

			return ustring16<TAlloc>::npos;
		}

		//! Finds next occurrence of character.
		//! \param c The character to search for.
		//! \param startPos The position in the string to start searching.
		//! \return Position where the character has been found, or ustring::npos if not found.
		size_t findNext(uchar32_t c, size_t startPos) const
		{
			const_iterator i(*this, startPos);

			size_t pos = startPos;
			while (!i.atEnd())
			{
				uchar32_t t = *i;
				if (t == c)
					return pos;
				++pos;
				++i;
			}

			return ustring16<TAlloc>::npos;
		}


		//! Finds last occurrence of character.
		//! \param c The character to search for.
		//! \param start The start position of the reverse search ( default = ustring::npos, on end ).
		//! \return Position where the character has been found, or ustring::npos if not found.
		size_t findLast(uchar32_t c, size_t start = ustring16<TAlloc>::npos) const
		{
			size_t s = size();
			if (start == ustring16<TAlloc>::npos)
				start = s - 1;

			const_iterator i(*this, start);
			size_t pos = start;
			while (!i.atStart())
			{
				uchar32_t t = *i;
				if (t == c)
					return pos;
				--pos;
				--i;
			}

			return ustring16<TAlloc>::npos;
		}

		//! Finds last occurrence of a character in a list.
		//! \param c A list of strings to find. For example if the method should find the last occurrence of 'a' or 'b', this parameter should be "ab".
		//! \param count The amount of characters in the list. Usually, this should be strlen(c).
		//! \return Position where one of the characters has been found, or -1 if not found.
		size_t findLastChar(const uchar32_t* const c, size_t count=1) const
		{
			if (!c || !count)
				return ustring16<TAlloc>::npos;

			const_iterator i(end());
			--i;

			size_t pos = size();
			while (!i.atStart())
			{
				uchar32_t t = *i;
				for (size_t j = 0; j < count; ++j)
					if (t == c[j])
						return pos;
				--pos;
				--i;
			}

			return ustring16<TAlloc>::npos;
		}


		//! Finds another ustring16 in this ustring16.
		//! \param str The string to find.
		//! \param start The start position of the search.
		//! \return Positions where the ustring16 has been found, or ustring::npos if not found.
		size_t find(const ustring16<TAlloc>& str, const size_t start = 0) const
		{
			size_t my_size = size();
			size_t their_size = str.size();

			if (their_size == 0 || my_size - start < their_size)
				return ustring16<TAlloc>::npos;

			const_iterator i(*this, start);

			size_t pos = start;
			while (!i.atEnd())
			{
				const_iterator i2(i);
				const_iterator j(str, 0);
				uchar32_t t1 = (uchar32_t)*i2;
				uchar32_t t2 = (uchar32_t)*j;
				while (t1 == t2)
				{
					++i2;
					++j;
					if (j.atEnd())
						return pos;
					t1 = (uchar32_t)*i2;
					t2 = (uchar32_t)*j;
				}
				++i;
				++pos;
			}

			return ustring16<TAlloc>::npos;
		}


		//! Finds another ustring16 in this ustring16.
		//! \param str The string to find.
		//! \param start The start position of the search.
		//! \return Positions where the string has been found, or -1 if not found.
		size_t find_raw(const ustring16<TAlloc>& str, const size_t start = 0) const
		{
			const uchar16_t* data = str.c_str();
			if (data && *data)
			{
				size_t len = 0;

				while (data[len])
					++len;

				if (len > used)
					return ustring16<TAlloc>::npos;

				for (size_t i = start; i <= used - len; ++i)
				{
					size_t j = 0;

					while(data[j] && elements[i+j] == data[j])
						++j;

					if (!data[j])
						return i;
				}
			}

			return ustring16<TAlloc>::npos;
		}


		//! Returns a substring.
		//! \param begin: Start of substring.
		//! \param length: Length of substring.
		//! \return A reference to our current string.
		ustring16<TAlloc> subString(size_t begin, size_t length) const
		{
			size_t len = size();

			// Check our inputs to see if we can exit early.
			if ((length == 0) || (begin>=len))
				return ustring16<TAlloc>("");

			// Clamp to maximum value.
			if (length == ustring16<TAlloc>::npos)
				length = len - begin;

			// TODO: Check for near overflow values.
			//if (begin > ustring16<TAlloc>::npos - length)

			// Clamp to the string length.
			if ((length + begin) > len)
				length = len - begin;

			ustring16<TAlloc> o;
			o.reserve((length+1) * 2);

			const_iterator i(*this, begin);
			while (!i.atEnd() && length)
			{
				o.append(*i);
				++i;
				--length;
			}

			return o;
		}

		//! Replaces all characters of a given type with another one.
		//! \param toReplace Character to replace.
		//! \param replaceWith Character replacing the old one.
		//! \return A reference to our current string.
		ustring16<TAlloc>& replace(uchar32_t toReplace, uchar32_t replaceWith)
		{
			iterator i(*this, 0);
			while (!i.atEnd())
			{
				typename ustring16<TAlloc>::access a = *i;
				if ((uchar32_t)a == toReplace)
					a = replaceWith;
				++i;
			}
			return *this;
		}


		//! Replaces all instances of a string with another one.
		//! \param toReplace The string to replace.
		//! \param replaceWith The string replacing the old one.
		//! \return A reference to our current string.
		ustring16<TAlloc>& replace(const ustring16<TAlloc>& toReplace, const ustring16<TAlloc>& replaceWith)
		{
			if (toReplace.size() == 0)
				return *this;

			const uchar16_t* other = toReplace.c_str();
			const uchar16_t* replace = replaceWith.c_str();
			const size_t other_size = toReplace.size_raw();
			const size_t replace_size = replaceWith.size_raw();

			// A character for character replace.  The string will not shrink or grow.
			if (replace_size == other_size)
			{
				size_t pos = 0;
				while ((pos = find_raw(other, pos)) != ustring16<TAlloc>::npos)
				{
					for (size_t i = 0; i < replace_size; ++i)
						elements[pos + i] = replace[i];
					++pos;
				}
				return *this;
			}

			// We are going to be removing some characters.  The string will shrink.
			if (replace_size < other_size)
			{
				size_t i = 0;
				for (size_t pos = 0; pos <= used; ++i, ++pos)
				{
					// Is this potentially a match?
					if (elements[pos] == *other)
					{
						// Check to see if we have a match.
						size_t j;
						for (j = 0; j < other_size; ++j)
						{
							if (elements[pos + j] != other[j])
								break;
						}

						// If we have a match, replace characters.
						if (j == other_size)
						{
							for (j = 0; j < replace_size; ++j)
								elements[i + j] = replace[j];
							i += replace_size - 1;
							pos += other_size - 1;
							continue;
						}
					}

					// No match found, just copy characters.
					elements[i - 1] = elements[pos];
				}
				elements[i] = 0;
				used = i;

				return *this;
			}

			// Determine the delta.
			size_t delta = replace_size - other_size;

			// We are going to be adding characters, so the string size will increase.
			// Count the number of times toReplace exists in the string so we can allocate the new size.
			size_t find_count = 0;
			size_t pos = 0;
			while ((pos = find_raw(other, pos)) != ustring16<TAlloc>::npos)
			{
				++find_count;
				++pos;
			}

			// Re-allocate the string now, if needed.
			size_t len = delta * find_count;
			if (used + len >= allocated)
				reallocate(used + len);

			// Start replacing.
			pos = 0;
			while ((pos = find_raw(other, pos)) != ustring16<TAlloc>::npos)
			{
				uchar16_t* start = elements + pos + other_size - 1;
				uchar16_t* ptr   = elements + used;
				uchar16_t* end   = elements + used + delta;

				// Shift characters to make room for the string.
				while (ptr != start)
				{
					*end = *ptr;
					--ptr;
					--end;
				}

				// Add the new string now.
				for (size_t i = 0; i < replace_size; ++i)
					elements[pos + i] = replace[i];

				pos += replace_size;
				used += delta;
			}

			// Terminate the string and return ourself.
			elements[used] = 0;
			return *this;
		}


		//! Removes characters from a ustring16..
		//! \param c The character to remove.
		//! \return A reference to our current string.
		ustring16<TAlloc>& remove(uchar32_t c)
		{
			size_t pos = 0;
			size_t found = 0;
			size_t len = (c > 0xFFFF ? 2 : 1);		// Remove characters equal to the size of c as a UTF-16 character.
			for (size_t i=0; i<=used; ++i)
			{
				uchar32_t uc32 = 0;
				if (!UTF16_IS_SURROGATE_HI(elements[i]))
					uc32 |= elements[i];
				else if (i + 1 <= used)
				{
					// Convert the surrogate pair into a single UTF-32 character.
					uc32 = unicode::toUTF32(elements[i], elements[i + 1]);
				}
				size_t len2 = (uc32 > 0xFFFF ? 2 : 1);

				if (uc32 == c)
				{
					found += len;
					continue;
				}

				elements[pos++] = elements[i];
				if (len2 == 2)
					elements[pos++] = elements[++i];
			}
			used -= found;
			elements[used] = 0;
			return *this;
		}


		//! Removes a ustring16 from the ustring16.
		//! \param toRemove The string to remove.
		//! \return A reference to our current string.
		ustring16<TAlloc>& remove(const ustring16<TAlloc>& toRemove)
		{
			size_t size = toRemove.size_raw();
			if (size == 0) return *this;

			const uchar16_t* tra = toRemove.c_str();
			size_t pos = 0;
			size_t found = 0;
			for (size_t i=0; i<=used; ++i)
			{
				size_t j = 0;
				while (j < size)
				{
					if (elements[i + j] != tra[j])
						break;
					++j;
				}
				if (j == size)
				{
					found += size;
					i += size - 1;
					continue;
				}

				elements[pos++] = elements[i];
			}
			used -= found;
			elements[used] = 0;
			return *this;
		}


		//! Removes characters from the ustring16.
		//! \param characters The characters to remove.
		//! \return A reference to our current string.
		ustring16<TAlloc>& removeChars(const ustring16<TAlloc>& characters)
		{
			if (characters.size_raw() == 0)
				return *this;

			size_t pos = 0;
			size_t found = 0;
			const_iterator iter(characters);
			for (size_t i=0; i<=used; ++i)
			{
				uchar32_t uc32 = 0;
				if (!UTF16_IS_SURROGATE_HI(elements[i]))
					uc32 |= elements[i];
				else if (i + 1 <= used)
				{
					// Convert the surrogate pair into a single UTF-32 character.
					uc32 = unicode::toUTF32(elements[i], elements[i+1]);
				}
				size_t len2 = (uc32 > 0xFFFF ? 2 : 1);

				bool cont = false;
				iter.toStart();
				while (!iter.atEnd())
				{
					uchar32_t c = *iter;
					if (uc32 == c)
					{
						found += (c > 0xFFFF ? 2 : 1);		// Remove characters equal to the size of c as a UTF-16 character.
						++i;
						cont = true;
						break;
					}
					++iter;
				}
				if (cont) continue;

				elements[pos++] = elements[i];
				if (len2 == 2)
					elements[pos++] = elements[++i];
			}
			used -= found;
			elements[used] = 0;
			return *this;
		}


		//! Trims the ustring16.
		//! Removes the specified characters (by default, Latin-1 whitespace) from the begining and the end of the ustring16.
		//! \param whitespace The characters that are to be considered as whitespace.
		//! \return A reference to our current string.
		ustring16<TAlloc>& trim(const ustring16<TAlloc>& whitespace = " \t\n\r")
		{
			core::array<uchar32_t> utf32white = whitespace.toUTF32();

			// find start and end of the substring without the specified characters
			const size_t begin = findFirstCharNotInList(utf32white.const_pointer(), whitespace.used + 1);
			if (begin == ustring16<TAlloc>::npos)
				return (*this="");

			const size_t end = findLastCharNotInList(utf32white.const_pointer(), whitespace.used + 1);
			if (end == ustring16<TAlloc>::npos)
				return (*this = subString(begin, ustring16<TAlloc>::npos));
			else return (*this = subString(begin, (end + 1) - begin));
		}


		//! Erases a character from the ustring16.
		//! May be slow, because all elements following after the erased element have to be copied.
		//! \param index Index of element to be erased.
		//! \return A reference to our current string.
		ustring16<TAlloc>& erase(size_t index)
		{
			YON_DEBUG_BREAK_IF(index>used) // access violation

				iterator i(*this, index);

			uchar32_t t = *i;
			size_t len = (t > 0xFFFF ? 2 : 1);

			for (size_t j = i.getPos() + len; j <= used; ++j)
				elements[j - len] = elements[j];

			used -= len;
			elements[used] = 0;

			return *this;
		}


		//! Validate the existing ustring16, checking for valid surrogate pairs and checking for proper termination.
		//! \return A reference to our current string.
		ustring16<TAlloc>& validate()
		{
			// Validate all unicode characters.
			for (size_t i=0; i<allocated; ++i)
			{
				// Terminate on existing null.
				if (elements[i] == 0)
				{
					used = i;
					return *this;
				}
				if (UTF16_IS_SURROGATE(elements[i]))
				{
					if (((i+1) >= allocated) || UTF16_IS_SURROGATE_LO(elements[i]))
						elements[i] = unicode::UTF_REPLACEMENT_CHARACTER;
					else if (UTF16_IS_SURROGATE_HI(elements[i]) && !UTF16_IS_SURROGATE_LO(elements[i+1]))
						elements[i] = unicode::UTF_REPLACEMENT_CHARACTER;
					++i;
				}
				if (elements[i] >= 0xFDD0 && elements[i] <= 0xFDEF)
					elements[i] = unicode::UTF_REPLACEMENT_CHARACTER;
			}

			// terminate
			used = 0;
			if (allocated > 0)
			{
				used = allocated - 1;
				elements[used] = 0;
			}
			return *this;
		}


		//! Gets the last char of the ustring16, or 0.
		//! \return The last char of the ustring16, or 0.
		uchar32_t lastChar() const
		{
			if (used < 1)
				return 0;

			if (UTF16_IS_SURROGATE_LO(elements[used-1]))
			{
				// Make sure we have a paired surrogate.
				if (used < 2)
					return 0;

				// Check for an invalid surrogate.
				if (!UTF16_IS_SURROGATE_HI(elements[used-2]))
					return 0;

				// Convert the surrogate pair into a single UTF-32 character.
				return unicode::toUTF32(elements[used-2], elements[used-1]);
			}
			else
			{
				return elements[used-1];
			}
		}


		//! Split the ustring16 into parts.
		/** This method will split a ustring16 at certain delimiter characters
		into the container passed in as reference. The type of the container
		has to be given as template parameter. It must provide a push_back and
		a size method.
		\param ret The result container
		\param c C-style ustring16 of delimiter characters
		\param count Number of delimiter characters
		\param ignoreEmptyTokens Flag to avoid empty substrings in the result
		container. If two delimiters occur without a character in between, an
		empty substring would be placed in the result. If this flag is set,
		only non-empty strings are stored.
		\param keepSeparators Flag which allows to add the separator to the
		result ustring16. If this flag is true, the concatenation of the
		substrings results in the original ustring16. Otherwise, only the
		characters between the delimiters are returned.
		\return The number of resulting substrings
		*/
		template<class container>
		size_t split(container& ret, const uchar32_t* const c, size_t count=1, bool ignoreEmptyTokens=true, bool keepSeparators=false) const
		{
			if (!c)
				return 0;

			const_iterator i(*this);
			const size_t oldSize=ret.size();
			size_t pos = 0;
			size_t lastpos = 0;
			size_t lastpospos = 0;
			bool lastWasSeparator = false;
			while (!i.atEnd())
			{
				uchar32_t ch = *i;
				bool foundSeparator = false;
				for (size_t j=0; j<count; ++j)
				{
					if (ch == c[j])
					{
						if ((!ignoreEmptyTokens || pos - lastpos != 0) &&
							!lastWasSeparator)
							ret.push_back(ustring16<TAlloc>(&elements[lastpospos], pos - lastpos));
						foundSeparator = true;
						lastpos = (keepSeparators ? pos : pos + 1);
						lastpospos = (keepSeparators ? i.getPos() : i.getPos() + 1);
						break;
					}
				}
				lastWasSeparator = foundSeparator;
				++pos;
				++i;
			}
			size_t s = size() + 1;
			if (s > lastpos)
				ret.push_back(ustring16<TAlloc>(&elements[lastpospos], s - lastpos));
			return ret.size()-oldSize;
		}


		//! Split the ustring16 into parts.
		/** This method will split a ustring16 at certain delimiter characters
		into the container passed in as reference. The type of the container
		has to be given as template parameter. It must provide a push_back and
		a size method.
		\param ret The result container
		\param c A unicode string of delimiter characters
		\param ignoreEmptyTokens Flag to avoid empty substrings in the result
		container. If two delimiters occur without a character in between, an
		empty substring would be placed in the result. If this flag is set,
		only non-empty strings are stored.
		\param keepSeparators Flag which allows to add the separator to the
		result ustring16. If this flag is true, the concatenation of the
		substrings results in the original ustring16. Otherwise, only the
		characters between the delimiters are returned.
		\return The number of resulting substrings
		*/
		template<class container>
		size_t split(container& ret, const ustring16<TAlloc>& c, bool ignoreEmptyTokens=true, bool keepSeparators=false) const
		{
			core::array<uchar32_t> v = c.toUTF32();
			return split(ret, v.pointer(), v.size(), ignoreEmptyTokens, keepSeparators);
		}


		//! Gets the size of the allocated memory buffer for the string.
		//! \return The size of the allocated memory buffer.
		size_t capacity() const
		{
			return allocated;
		}


		//! Returns the raw number of UTF-16 code points in the string which includes the individual surrogates.
		//! \return The raw number of UTF-16 code points, excluding the trialing NUL.
		size_t size_raw() const
		{
			return used;
		}


		//! Inserts a character into the string.
		//! \param c The character to insert.
		//! \param pos The position to insert the character.
		//! \return A reference to our current string.
		ustring16<TAlloc>& insert(uchar32_t c, size_t pos)
		{
			u8 len = (c > 0xFFFF ? 2 : 1);

			if (used + len >= allocated)
				reallocate(used + len);

			used += len;

			iterator iter(*this, pos);
			for (size_t i = used - 2; i > iter.getPos(); --i)
				elements[i] = elements[i - len];

			if (c > 0xFFFF)
			{
				// c will be multibyte, so split it up into a surrogate pair.
				uchar16_t x = static_cast<uchar16_t>(c);
				uchar16_t vh = UTF16_HI_SURROGATE | ((((c >> 16) & ((1 << 5) - 1)) - 1) << 6) | (x >> 10);
				uchar16_t vl = UTF16_LO_SURROGATE | (x & ((1 << 10) - 1));
				elements[iter.getPos()] = vh;
				elements[iter.getPos()+1] = vl;
			}
			else
			{
				elements[iter.getPos()] = static_cast<uchar16_t>(c);
			}
			elements[used] = 0;
			return *this;
		}


		//! Inserts a string into the string.
		//! \param c The string to insert.
		//! \param pos The position to insert the string.
		//! \return A reference to our current string.
		ustring16<TAlloc>& insert(const ustring16<TAlloc>& c, size_t pos)
		{
			size_t len = c.size_raw();
			if (len == 0) return *this;

			if (used + len >= allocated)
				reallocate(used + len);

			used += len;

			iterator iter(*this, pos);
			for (size_t i = used - 2; i > iter.getPos() + len; --i)
				elements[i] = elements[i - len];

			const uchar16_t* s = c.c_str();
			for (size_t i = 0; i < len; ++i)
			{
				elements[pos++] = *s;
				++s;
			}

			elements[used] = 0;
			return *this;
		}


		//! Inserts a character into the string.
		//! \param c The character to insert.
		//! \param pos The position to insert the character.
		//! \return A reference to our current string.
		ustring16<TAlloc>& insert_raw(uchar16_t c, size_t pos)
		{
			if (used + 1 >= allocated)
				reallocate(used + 1);

			++used;

			for (size_t i = used - 1; i > pos; --i)
				elements[i] = elements[i - 1];

			elements[pos] = c;
			elements[used] = 0;
			return *this;
		}


		//! Removes a character from string.
		//! \param pos Position of the character to remove.
		//! \return A reference to our current string.
		ustring16<TAlloc>& erase_raw(size_t pos)
		{
			for (size_t i=pos; i<=used; ++i)
			{
				elements[i] = elements[i + 1];
			}
			--used;
			elements[used] = 0;
			return *this;
		}


		//! Replaces a character in the string.
		//! \param c The new character.
		//! \param pos The position of the character to replace.
		//! \return A reference to our current string.
		ustring16<TAlloc>& replace_raw(uchar16_t c, size_t pos)
		{
			elements[pos] = c;
			return *this;
		}


		//! Returns an iterator to the beginning of the string.
		//! \return An iterator to the beginning of the string.
		iterator begin()
		{
			iterator i(*this, 0);
			return i;
		}


		//! Returns an iterator to the beginning of the string.
		//! \return An iterator to the beginning of the string.
		const_iterator begin() const
		{
			const_iterator i(*this, 0);
			return i;
		}


		//! Returns an iterator to the beginning of the string.
		//! \return An iterator to the beginning of the string.
		const_iterator cbegin() const
		{
			const_iterator i(*this, 0);
			return i;
		}


		//! Returns an iterator to the end of the string.
		//! \return An iterator to the end of the string.
		iterator end()
		{
			iterator i(*this, 0);
			i.toEnd();
			return i;
		}


		//! Returns an iterator to the end of the string.
		//! \return An iterator to the end of the string.
		const_iterator end() const
		{
			const_iterator i(*this, 0);
			i.toEnd();
			return i;
		}


		//! Returns an iterator to the end of the string.
		//! \return An iterator to the end of the string.
		const_iterator cend() const
		{
			const_iterator i(*this, 0);
			i.toEnd();
			return i;
		}

		//! Converts the string to a UTF-8 encoded string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return A string containing the UTF-8 encoded string.
		core::stringc toStringc(const bool addBOM = false) const
		{
			core::stringc ret;
			ret.reserve((used * 4 + (addBOM ? unicode::BOM_UTF8_LEN : 0) + 1));
			const_iterator iter(*this, 0);

			// Add the byte order mark if the user wants it.
			if (addBOM)
			{
				ret.append((c8)unicode::BOM_ENCODE_UTF8[0]);
				ret.append((c8)unicode::BOM_ENCODE_UTF8[1]);
				ret.append((c8)unicode::BOM_ENCODE_UTF8[2]);
			}

			while (!iter.atEnd())
			{
				uchar32_t c = *iter;
				if (c > 0xFFFF)
				{	// 4 bytes
					uchar8_t b1 = (0x1E << 3) | ((c >> 18) & 0x7);
					uchar8_t b2 = (0x2 << 6) | ((c >> 12) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b4 = (0x2 << 6) | (c & 0x3F);
					ret.append((c8)b1);
					ret.append((c8)b2);
					ret.append((c8)b3);
					ret.append((c8)b4);
				}
				else if (c > 0x7FF)
				{	// 3 bytes
					uchar8_t b1 = (0xE << 4) | ((c >> 12) & 0xF);
					uchar8_t b2 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | (c & 0x3F);
					ret.append((c8)b1);
					ret.append((c8)b2);
					ret.append((c8)b3);
				}
				else if (c > 0x7F)
				{	// 2 bytes
					uchar8_t b1 = (0x6 << 5) | ((c >> 6) & 0x1F);
					uchar8_t b2 = (0x2 << 6) | (c & 0x3F);
					ret.append((c8)b1);
					ret.append((c8)b2);
				}
				else
				{	// 1 byte
					ret.append(static_cast<c8>(c));
				}
				++iter;
			}
			return ret;
		}


		//! Converts the string to a UTF-8 encoded string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return A string containing the UTF-8 encoded string.
		core::string<uchar8_t> toUTF8_s(const bool addBOM = false) const
		{
			core::string<uchar8_t> ret;
			ret.reserve(used * 4 + (addBOM ? unicode::BOM_UTF8_LEN : 0) + 1);
			const_iterator iter(*this, 0);

			// Add the byte order mark if the user wants it.
			if (addBOM)
			{
				ret.append(unicode::BOM_ENCODE_UTF8[0]);
				ret.append(unicode::BOM_ENCODE_UTF8[1]);
				ret.append(unicode::BOM_ENCODE_UTF8[2]);
			}

			while (!iter.atEnd())
			{
				uchar32_t c = *iter;
				if (c > 0xFFFF)
				{	// 4 bytes
					uchar8_t b1 = (0x1E << 3) | ((c >> 18) & 0x7);
					uchar8_t b2 = (0x2 << 6) | ((c >> 12) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b4 = (0x2 << 6) | (c & 0x3F);
					ret.append(b1);
					ret.append(b2);
					ret.append(b3);
					ret.append(b4);
				}
				else if (c > 0x7FF)
				{	// 3 bytes
					uchar8_t b1 = (0xE << 4) | ((c >> 12) & 0xF);
					uchar8_t b2 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | (c & 0x3F);
					ret.append(b1);
					ret.append(b2);
					ret.append(b3);
				}
				else if (c > 0x7F)
				{	// 2 bytes
					uchar8_t b1 = (0x6 << 5) | ((c >> 6) & 0x1F);
					uchar8_t b2 = (0x2 << 6) | (c & 0x3F);
					ret.append(b1);
					ret.append(b2);
				}
				else
				{	// 1 byte
					ret.append(static_cast<uchar8_t>(c));
				}
				++iter;
			}
			return ret;
		}


#ifdef USTRING_CPP0X_NEWLITERALS	// C++0x
		//! Converts the string to a UTF-16 encoded string.
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return A string containing the UTF-16 encoded string.
		core::string<char16_t> toUTF16_s(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			core::string<char16_t> ret;
			ret.reserve(used + (addBOM ? unicode::BOM_UTF16_LEN : 0) + 1);

			// Add the BOM if specified.
			if (addBOM)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE)
					ret[0] = unicode::BOM;
				else if (endian == ENUM_UTF_ENDIAN_LITTLE)
				{
					uchar8_t* ptr8 = reinterpret_cast<uchar8_t*>(ret.c_str());
					*ptr8++ = unicode::BOM_ENCODE_UTF16LE[0];
					*ptr8 = unicode::BOM_ENCODE_UTF16LE[1];
				}
				else
				{
					uchar8_t* ptr8 = reinterpret_cast<uchar8_t*>(ret.c_str());
					*ptr8++ = unicode::BOM_ENCODE_UTF16BE[0];
					*ptr8 = unicode::BOM_ENCODE_UTF16BE[1];
				}
			}

			ret.append(elements);
			if (endian != ENUM_UTF_ENDIAN_NATIVE && getEndianness() != endian)
			{
				char16_t* ptr = ret.c_str();
				for (size_t i = 0; i < ret.size(); ++i)
					*ptr++ = unicode::swapEndian16(*ptr);
			}
			return ret;
		}

		//! Converts the string to a UTF-32 encoded string.
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return A string containing the UTF-32 encoded string.
		core::string<char32_t> toUTF32_s(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			core::string<char32_t> ret;
			ret.reserve(size() + 1 + (addBOM ? unicode::BOM_UTF32_LEN : 0));
			const_iterator iter(*this, 0);

			// Add the BOM if specified.
			if (addBOM)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE)
					ret.append(unicode::BOM);
				else
				{
					union
					{
						uchar32_t full;
						u8 chunk[4];
					} t;

					if (endian == ENUM_UTF_ENDIAN_LITTLE)
					{
						t.chunk[0] = unicode::BOM_ENCODE_UTF32LE[0];
						t.chunk[1] = unicode::BOM_ENCODE_UTF32LE[1];
						t.chunk[2] = unicode::BOM_ENCODE_UTF32LE[2];
						t.chunk[3] = unicode::BOM_ENCODE_UTF32LE[3];
					}
					else
					{
						t.chunk[0] = unicode::BOM_ENCODE_UTF32BE[0];
						t.chunk[1] = unicode::BOM_ENCODE_UTF32BE[1];
						t.chunk[2] = unicode::BOM_ENCODE_UTF32BE[2];
						t.chunk[3] = unicode::BOM_ENCODE_UTF32BE[3];
					}
					ret.append(t.full);
				}
			}

			while (!iter.atEnd())
			{
				uchar32_t c = *iter;
				if (endian != ENUM_UTF_ENDIAN_NATIVE && getEndianness() != endian)
					c = unicode::swapEndian32(c);
				ret.append(c);
				++iter;
			}
			return ret;
		}
#endif

#if 0

		//! Converts the string to a UTF-8 encoded string array.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return An array containing the UTF-8 encoded string.
		core::array<uchar8_t> toUTF8(const bool addBOM = false) const
		{
			core::array<uchar8_t> ret(used * 4 + (addBOM ? unicode::BOM_UTF8_LEN : 0) + 1);
			const_iterator iter(*this, 0);

			// Add the byte order mark if the user wants it.
			if (addBOM)
			{
				ret.push_back(unicode::BOM_ENCODE_UTF8[0]);
				ret.push_back(unicode::BOM_ENCODE_UTF8[1]);
				ret.push_back(unicode::BOM_ENCODE_UTF8[2]);
			}

			while (!iter.atEnd())
			{
				uchar32_t c = *iter;
				if (c > 0xFFFF)
				{	// 4 bytes
					uchar8_t b1 = (0x1E << 3) | ((c >> 18) & 0x7);
					uchar8_t b2 = (0x2 << 6) | ((c >> 12) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b4 = (0x2 << 6) | (c & 0x3F);
					ret.push_back(b1);
					ret.push_back(b2);
					ret.push_back(b3);
					ret.push_back(b4);
				}
				else if (c > 0x7FF)
				{	// 3 bytes
					uchar8_t b1 = (0xE << 4) | ((c >> 12) & 0xF);
					uchar8_t b2 = (0x2 << 6) | ((c >> 6) & 0x3F);
					uchar8_t b3 = (0x2 << 6) | (c & 0x3F);
					ret.push_back(b1);
					ret.push_back(b2);
					ret.push_back(b3);
				}
				else if (c > 0x7F)
				{	// 2 bytes
					uchar8_t b1 = (0x6 << 5) | ((c >> 6) & 0x1F);
					uchar8_t b2 = (0x2 << 6) | (c & 0x3F);
					ret.push_back(b1);
					ret.push_back(b2);
				}
				else
				{	// 1 byte
					ret.push_back(static_cast<uchar8_t>(c));
				}
				++iter;
			}
			ret.push_back(0);
			return ret;
		}


		//! Converts the string to a UTF-16 encoded string array.
		//! Unfortunately, no toUTF16_s() version exists due to limitations with Irrlicht's string class.
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return An array containing the UTF-16 encoded string.
		core::array<uchar16_t> toUTF16(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			core::array<uchar16_t> ret(used + (addBOM ? unicode::BOM_UTF16_LEN : 0) + 1);
			uchar16_t* ptr = ret.pointer();

			// Add the BOM if specified.
			if (addBOM)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE)
					*ptr = unicode::BOM;
				else if (endian == ENUM_UTF_ENDIAN_LITTLE)
				{
					uchar8_t* ptr8 = reinterpret_cast<uchar8_t*>(ptr);
					*ptr8++ = unicode::BOM_ENCODE_UTF16LE[0];
					*ptr8 = unicode::BOM_ENCODE_UTF16LE[1];
				}
				else
				{
					uchar8_t* ptr8 = reinterpret_cast<uchar8_t*>(ptr);
					*ptr8++ = unicode::BOM_ENCODE_UTF16BE[0];
					*ptr8 = unicode::BOM_ENCODE_UTF16BE[1];
				}
				++ptr;
			}

			memcpy((void*)ptr, (void*)elements, used * sizeof(uchar16_t));
			if (endian != ENUM_UTF_ENDIAN_NATIVE && getEndianness() != endian)
			{
				for (size_t i = 0; i <= used; ++i)
					*ptr++ = unicode::swapEndian16(*ptr);
			}
			ret.set_used(used + (addBOM ? unicode::BOM_UTF16_LEN : 0));
			ret.push_back(0);
			return ret;
		}


		//! Converts the string to a UTF-32 encoded string array.
		//! Unfortunately, no toUTF32_s() version exists due to limitations with Irrlicht's string class.
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return An array containing the UTF-32 encoded string.
		core::array<uchar32_t> toUTF32(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			core::array<uchar32_t> ret(size() + (addBOM ? unicode::BOM_UTF32_LEN : 0) + 1);
			const_iterator iter(*this, 0);

			// Add the BOM if specified.
			if (addBOM)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE)
					ret.push_back(unicode::BOM);
				else
				{
					union
					{
						uchar32_t full;
						u8 chunk[4];
					} t;

					if (endian == ENUM_UTF_ENDIAN_LITTLE)
					{
						t.chunk[0] = unicode::BOM_ENCODE_UTF32LE[0];
						t.chunk[1] = unicode::BOM_ENCODE_UTF32LE[1];
						t.chunk[2] = unicode::BOM_ENCODE_UTF32LE[2];
						t.chunk[3] = unicode::BOM_ENCODE_UTF32LE[3];
					}
					else
					{
						t.chunk[0] = unicode::BOM_ENCODE_UTF32BE[0];
						t.chunk[1] = unicode::BOM_ENCODE_UTF32BE[1];
						t.chunk[2] = unicode::BOM_ENCODE_UTF32BE[2];
						t.chunk[3] = unicode::BOM_ENCODE_UTF32BE[3];
					}
					ret.push_back(t.full);
				}
			}
			ret.push_back(0);

			while (!iter.atEnd())
			{
				uchar32_t c = *iter;
				if (endian != ENUM_UTF_ENDIAN_NATIVE && getEndianness() != endian)
					c = unicode::swapEndian32(c);
				ret.push_back(c);
				++iter;
			}
			return ret;
		}

		//! Converts the string to a wchar_t encoded string array.
		/** The size of a wchar_t changes depending on the platform.  This function will store a
		correct UTF-8, -16, or -32 encoded string depending on the size of a wchar_t. **/
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return An array containing the wchar_t encoded string.
		core::array<wchar_t> toWCHAR(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			if (sizeof(wchar_t) == 4)
			{
				core::array<uchar32_t> a(toUTF32(endian, addBOM));
				core::array<wchar_t> ret(a.size());
				ret.set_used(a.size());
				memcpy((void*)ret.pointer(), (void*)a.pointer(), a.size() * sizeof(uchar32_t));
				return ret;
			}
			if (sizeof(wchar_t) == 2)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE && addBOM == false)
				{
					core::array<wchar_t> ret(used);
					ret.set_used(used);
					memcpy((void*)ret.pointer(), (void*)elements, used * sizeof(uchar16_t));
					return ret;
				}
				else
				{
					core::array<uchar16_t> a(toUTF16(endian, addBOM));
					core::array<wchar_t> ret(a.size());
					ret.set_used(a.size());
					memcpy((void*)ret.pointer(), (void*)a.pointer(), a.size() * sizeof(uchar16_t));
					return ret;
				}
			}
			if (sizeof(wchar_t) == 1)
			{
				core::array<uchar8_t> a(toUTF8(addBOM));
				core::array<wchar_t> ret(a.size());
				ret.set_used(a.size());
				memcpy((void*)ret.pointer(), (void*)a.pointer(), a.size() * sizeof(uchar8_t));
				return ret;
			}

			// Shouldn't happen.
			return core::array<wchar_t>();
		}



		//! Converts the string to a wchar_t encoded string.
		/** The size of a wchar_t changes depending on the platform.  This function will store a
		correct UTF-8, -16, or -32 encoded string depending on the size of a wchar_t. **/
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return A string containing the wchar_t encoded string.
		core::string<wchar_t> toWCHAR_s(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
			if (sizeof(wchar_t) == 4)
			{
				core::array<uchar32_t> a(toUTF32(endian, addBOM));
				core::stringw ret(a.pointer());
				return ret;
			}
			else if (sizeof(wchar_t) == 2)
			{
				if (endian == ENUM_UTF_ENDIAN_NATIVE && addBOM == false)
				{
					core::stringw ret(elements);
					return ret;
				}
				else
				{
					core::array<uchar16_t> a(toUTF16(endian, addBOM));
					core::stringw ret(a.pointer());
					return ret;
				}
			}
			else if (sizeof(wchar_t) == 1)
			{
				core::array<uchar8_t> a(toUTF8(addBOM));
				core::stringw ret(a.pointer());
				return ret;
			}

			// Shouldn't happen.
			return core::stringw();
		}

		//! Converts the string to a properly encoded io::path string.
		//! \param endian The desired endianness of the string.
		//! \param addBOM If true, the proper unicode byte-order mark will be prefixed to the string.
		//! \return An io::path string containing the properly encoded string.
		io::path toPATH_s(const ENUM_UTF_ENDIAN endian = ENUM_UTF_ENDIAN_NATIVE, const bool addBOM = false) const
		{
#if defined(YON_WCHAR_FILESYSTEM)
			return toWCHAR_s(endian, addBOM);
#else
			return toUTF8_s(addBOM);
#endif
		}

#endif

		//! Loads an unknown stream of data.
		//! Will attempt to determine if the stream is unicode data.  Useful for loading from files.
		//! \param data The data stream to load from.
		//! \param data_size The length of the data string.
		//! \return A reference to our current string.
		ustring16<TAlloc>& build(const char* data, size_t data_size)
		{
			// Clear our string.
			*this = "";
			if (!data)
				return *this;

			ENUM_UTF_ENCODING e = unicode::determineUnicodeBOM(data);
			switch (e)
			{
			default:
			case ENUM_UTF_ENCODING_UTF8:
				append((uchar8_t*)data, data_size);
				break;

			case ENUM_UTF_ENCODING_UTF16:
			case ENUM_UTF_ENCODING_UTF16BE:
			case ENUM_UTF_ENCODING_UTF16LE:
				append((uchar16_t*)data, data_size / 2);
				break;

			case ENUM_UTF_ENCODING_UTF32:
			case ENUM_UTF_ENCODING_UTF32BE:
			case ENUM_UTF_ENCODING_UTF32LE:
				append((uchar32_t*)data, data_size / 4);
				break;
			}

			return *this;
		}

		//! Gets the encoding of the Unicode string this class contains.
		//! \return An enum describing the current encoding of this string.
		const ENUM_UTF_ENCODING getEncoding() const
		{
			return encoding;
		}

		//! Gets the endianness of the Unicode string this class contains.
		//! \return An enum describing the endianness of this string.
		const ENUM_UTF_ENDIAN getEndianness() const
		{
			if (encoding ==ENUM_UTF_ENCODING_UTF16LE ||
				encoding ==ENUM_UTF_ENCODING_UTF32LE)
				return ENUM_UTF_ENDIAN_LITTLE;
			else return ENUM_UTF_ENDIAN_BIG;
		}

	private:

		//! Reallocate the string, making it bigger or smaller.
		//! \param new_size The new size of the string.
		void reallocate(size_t new_size)
		{
			uchar16_t* old_elements = elements;

			elements = allocator.allocate(new_size + 1); //new u16[new_size];
			allocated = new_size + 1;
			if (old_elements == 0) 
				return;

			size_t amount = used < new_size ? used : new_size;
			for (size_t i=0; i<=amount; ++i)
				elements[i] = old_elements[i];

			if (allocated <= used)
				used = allocated - 1;

			elements[used] = 0;

			allocator.deallocate(old_elements); // delete [] old_elements;
		}

		//--- member variables

		uchar16_t* elements;
		ENUM_UTF_ENCODING encoding;
		size_t allocated;
		size_t used;
		TAlloc allocator;
	};

	typedef ustring16<yonAllocator<uchar16_t> > ustring;
}
}
#endif