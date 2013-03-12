#ifndef _YON_GUI_VARIANT_H_
#define _YON_GUI_VARIANT_H_

#include "yonString.h"
#include "vector2d.h"
#include "SColor.h"

namespace yon{
namespace gui{

	/**
	Variant is a container that can store a selection of basic types. The variant will store the
	value in the native form corresponding to the version of Set that was called.

	Get is templated to convert from the stored form to the requested form by using a TypeConverter.
	*/
	class variant{
	private:

		//TODO 确认位数
#ifdef YON_ARCH_BITMODE_64
		static const s32 LOCAL_DATA_SIZE = 32; // Required for Strings
#else
		static const s32 LOCAL_DATA_SIZE = 24;
#endif
		ENUM_TYPE Type;
		c8 Data[LOCAL_DATA_SIZE];
	public:
		/// Type of data stored in the variant.
		enum ENUM_TYPE
		{
			NONE = '-',
			BYTE = 'b',
			CHAR = 'c',
			FLOAT = 'f',
			INT = 'i', 
			STRING = 's',
			WORD = 'w',
			VECTOR2 = '2',
			COLOURF = 'g',
			COLOURB = 'h',
			//SCRIPTINTERFACE = 'p',
			VOIDPTR = '*',			
		};

		variant();

		explicit variant(const)
		/// Templatised constructors don't work for the copy constructor, so we have to define it
		/// explicitly.
		variant(const variant&);
		/// Constructs a variant with internal data.
		/// @param[in] t Data of a supported type to set on the variant.
		template< typename T >
		variant(const T& t);

		/// Clears the data structure stored by the variant.
		void clear();

		/// Gets the current internal representation type.
		/// @return The type of data stored in the variant internally.
		ENUM_TYPE getType() const;

		/// Shares another variant's data with this variant.
		/// @param[in] copy Variant to share data.
		void setValue(const variant& copy);
		/// Sets a byte value on this variant.
		/// @param[in] value New value to set.
		void setValue(const u8 value);
		/// Sets a signed char value on this variant.
		/// @param[in] value New value to set.
		void setValue(const c8 value);
		/// Sets a float value on this variant.
		/// @param[in] value New value to set.
		void setValue(const f32 value);
		/// Sets a signed int value on this variant.
		/// @param[in] value New value to set.
		void setValue(const s32 value);
		/// Sets a word value on this variant.
		/// @param[in] value New value to set.
		void setValue(const u16 value);
		/// Sets a constant C string value on this variant.
		/// @param[in] value New value to set.
		void setValue(const char* value);
		/// Sets a generic void* value on this variant.
		/// @param[in] value New value to set.
		void setValue(pvoid value);
		/// Sets an EMP string value on this variant.
		/// @param[in] value New value to set.
		void setValue(const core::stringc& value);
		/// Sets a Vector2f value on this variant.
		/// @param[in] value New value to set.
		void setValue(const core::vector2df& value);
		/// Sets a Colourf value on this variant.
		/// @param[in] value New value to set.
		void setValue(const video::SColorf& value);
		/// Sets a Colourb value on this variant.
		/// @param[in] value New value to set.
		void setValue(const video::SColor& value);
		/// Sets a script object value on this variant.
		/// @param[in] value New value to set.
		//void setValue(ScriptInterface* value);

		/// Templatised data accessor. TypeConverters will be used to attempt to convert from the
		/// internal representation to the requested representation.
		/// @return Data in the requested type.
		template< typename T >
		T getValue() const;

		/// Templatised data accessor. TypeConverters will be used to attempt to convert from the
		/// internal representation to the requested representation.
		/// @param[out] value Data in the requested type.
		/// @return True if the value was converted and returned, false if no data was stored in the variant.
		template< typename T >
		bool getValue(T& value) const;

		/// Assigns another variant's internal data to this variant.
		/// @param[in] copy Variant to share data.
		variant& operator=(const variant& copy);

	};

	variant::variant() : Type(NONE)
	{
		// Make sure our object size assumptions fit inside the static buffer
		YON_DEBUG_BREAK_IF(sizeof(video::SColor) <= LOCAL_DATA_SIZE);
		YON_DEBUG_BREAK_IF(sizeof(video::SColorf) <= LOCAL_DATA_SIZE);
		YON_DEBUG_BREAK_IF(sizeof(core::stringc) <= LOCAL_DATA_SIZE);
	}

	variant::variant( const variant& copy ) : Type(NONE)
	{
		setValue(copy);
	}

	variant::~variant() 
	{
		clear();
	}

	void variant::clear()
	{
		// Free any allocated types.
		switch (Type) 
		{      
		case STRING:
			{
				// Clean up the string.
				core::stringc* str = (core::stringc*)Data;
				str->~stringc();
			}
			break;

		default:
			break;
		}
		Type = NONE;
	}

	variant::ENUM_TYPE variant::getType() const
	{
		return Type;
	}

	//////////////////////////////////////////////////
	// Set methods
	//////////////////////////////////////////////////

#define SET_VARIANT(type) *((type*)data) = value;

	void variant::setValue(const variant& copy)
	{
		switch (copy.Type)
		{
		case STRING:
			{
				// Create the string
				setValue(*(core::stringc*)copy.Data);
			}
			break;

		default:
			clear();
			memcpy(Data, copy.Data, LOCAL_DATA_SIZE);
			break;	
		}
		Type = copy.Type;
	}

	void variant::setValue(const u8 value)
	{
		Type = BYTE;
		SET_VARIANT(u8);
	}

	void variant::setValue(const c8 value)
	{
		Type = CHAR;
		SET_VARIANT(c8);
	}

	void variant::setValue(const f32 value)
	{
		Type = FLOAT;
		SET_VARIANT(f32);
	}

	void variant::setValue(const s32 value)
	{
		Type = INT;
		SET_VARIANT(s32);
	}

	void variant::setValue(const core::stringc& value) 
	{
		if (Type == STRING)
		{
			((core::stringc*)Data)=value;
		}
		else
		{
			Type = STRING;
			//TODO why not new(m_data) value;
			new(Data) core::stringc(value);
		}
	}

	void variant::setValue(const u16 value)
	{
		Type = WORD;
		SET_VARIANT(u16);  
	}

	void variant::setValue(const c8* value) 
	{
		setValue(core::stringc(value));
	}

	void variant::setValue(void* voidptr) 
	{
		Type = VOIDPTR;
		memcpy(Data, &voidptr, sizeof(void*));
	}

	void variant::setValue(const core::vector2df& value)
	{
		Type = VECTOR2;
		SET_VARIANT(core::vector2df);
	}

	void variant::setValue(const video::SColorf& value)
	{
		Type = COLOURF;
		SET_VARIANT(video::SColorf);
	}

	void variant::setValue(const video::SColor& value)
	{
		Type = COLOURB;
		SET_VARIANT(video::SColor);
	}

	/*void variant::setValue(ScriptInterface* value) 
	{
		type = SCRIPTINTERFACE;
		memcpy(data, &value, sizeof(ScriptInterface*));
	}*/

	variant& variant::operator=(const variant& copy)
	{
		setValue(copy);
		return *this;
	}

	// Constructs a variant with internal data.
	template< typename T >
	variant::variant(const T& t) : m_type(NONE)
	{
		setValue( t );
	}

	// Templatised data accessor.
	template< typename T >
	bool variant::getValue(T& value) const
	{	
		switch (Type)
		{
		case BYTE:
			return core::convertor< u8, T >::convert(*(u8*)Data, value);
			break;

		case CHAR:
			return core::convertor< c8, T >::convert(*(c8*)Data, value);
			break;

		case FLOAT:
			return core::convertor< f32, T >::convert(*(f32*)Data, value);
			break;

		case INT:
			return core::convertor< s32, T >::convert(*(s32*)Data, value);
			break;

		case STRING:
			return core::convertor< core::stringc, T >::convert(*(core::stringc*)Data, value);
			break;

		case WORD:
			return core::convertor< u16, T >::convert(*(u16*)Data, value);
			break;

		case VECTOR2:
			return core::convertor< core::vector2df, T >::convert(*(core::vector2df*)Data, value);
			break;

		case COLOURF:
			return core::convertor< video::SColorf, T >::convert(*(video::SColorf*)Data, value);
			break;

		case COLOURB:
			return core::convertor< video::SColor, T >::convert(*(video::SColor*)Data, value);
			break;

		/*case SCRIPTINTERFACE:
			return TypeConverter< ScriptInterface*, T >::Convert((ScriptInterface*)data, value);			
			break;*/

		case VOIDPTR:
			return core::convertor< pvoid, T >::convert((void*)Data, value);
			break;

		case NONE:
			break;

		}

		return false;
	}

	// Templatised data accessor.
	template< typename T >
	T variant::getValue() const
	{
		T value;
		getValue(value);
		return value;
	}

#undef SET_VARIANT
}
}
#endif