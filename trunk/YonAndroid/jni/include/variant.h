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

#ifdef YON_ARCH_BITMODE_64
		static const s32 LOCAL_DATA_SIZE = 32; // Required for Strings
#else
		static const s32 LOCAL_DATA_SIZE = 24;
#endif
		Type m_type;
		c8 m_data[LOCAL_DATA_SIZE];
	public:
		/// Type of data stored in the variant.
		enum Type
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
			SCRIPTINTERFACE = 'p',
			VOIDPTR = '*',			
		};

		variant();
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
		Type getType() const;

		/// Shares another variant's data with this variant.
		/// @param[in] copy Variant to share data.
		void setValue(const variant& copy);

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

	variant::variant() : m_type(NONE)
	{
		// Make sure our object size assumptions fit inside the static buffer
		YON_DEBUG_BREAK_IF(sizeof(video::SColor) <= LOCAL_DATA_SIZE);
		YON_DEBUG_BREAK_IF(sizeof(video::SColorf) <= LOCAL_DATA_SIZE);
		YON_DEBUG_BREAK_IF(sizeof(core::stringc) <= LOCAL_DATA_SIZE);
	}

	variant::variant( const variant& copy ) : m_type(NONE)
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
		switch (type) 
		{      
		case STRING:
			{
				// Clean up the string.
				core::stringc* str = (core::stringc*)m_data;
				str->~stringc();
			}
			break;

		default:
			break;
		}
		type = NONE;
	}

	variant::Type variant::getType() const
	{
		return m_type;
	}

	//////////////////////////////////////////////////
	// Set methods
	//////////////////////////////////////////////////

#define SET_VARIANT(type) *((type*)data) = value;

	void variant::setValue(const variant& copy)
	{
		switch (copy.m_type)
		{
		case STRING:
			{
				// Create the string
				setValue(*(core::stringc*)copy.m_data);
			}
			break;

		default:
			clear();
			memcpy(m_data, copy.m_data, LOCAL_DATA_SIZE);
			break;	
		}
		m_type = copy.m_type;
	}

	void variant::setValue(const u8 value)
	{
		type = BYTE;
		SET_VARIANT(u8);
	}

	void variant::setValue(const c8 value)
	{
		type = CHAR;
		SET_VARIANT(c8);
	}

	void variant::setValue(const f32 value)
	{
		type = FLOAT;
		SET_VARIANT(f32);
	}

	void variant::setValue(const s32 value)
	{
		type = INT;
		SET_VARIANT(s32);
	}

	void variant::setValue(const core::stringc& value) 
	{
		if (type == STRING)
		{
			((core::stringc*)m_data)=value;
		}
		else
		{
			type = STRING;
			//TODO why not new(m_data) value;
			new(m_data) core::stringc(value);
		}
	}

	void variant::setValue(const u16 value)
	{
		type = WORD;
		SET_VARIANT(u16);  
	}

	void variant::setValue(const c8* value) 
	{
		setValue(core::stringc(value));
	}

	void variant::setValue(void* voidptr) 
	{
		type = VOIDPTR;
		memcpy(m_data, &voidptr, sizeof(void*));
	}

	void variant::setValue(const core::vector2df& value)
	{
		type = VECTOR2;
		SET_VARIANT(core::vector2df);
	}

	void variant::setValue(const video::SColorf& value)
	{
		type = COLOURF;
		SET_VARIANT(video::SColorf);
	}

	void variant::setValue(const video::SColor& value)
	{
		type = COLOURB;
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
		switch (type)
		{
		case BYTE:
			return TypeConverter< u8, T >::Convert(*(byte*)data, value);
			break;

		case CHAR:
			return TypeConverter< c8, T >::Convert(*(char*)data, value);
			break;

		case FLOAT:
			return TypeConverter< f32, T >::Convert(*(float*)data, value);
			break;

		case INT:
			return TypeConverter< s32, T >::Convert(*(int*)data, value);
			break;

		case STRING:
			return TypeConverter< core::stringc, T >::Convert(*(String*)data, value);
			break;

		case WORD:
			return TypeConverter< u16, T >::Convert(*(word*)data, value);
			break;

		case VECTOR2:
			return TypeConverter< core::vector2df, T >::Convert(*(Vector2f*)data, value);
			break;

		case COLOURF:
			return TypeConverter< video::SColorf, T >::Convert(*(Colourf*)data, value);
			break;

		case COLOURB:
			return TypeConverter< video::SColor, T >::Convert(*(Colourb*)data, value);
			break;

		/*case SCRIPTINTERFACE:
			return TypeConverter< ScriptInterface*, T >::Convert((ScriptInterface*)data, value);			
			break;*/

		case VOIDPTR:
			return TypeConverter< void*, T >::Convert((void*)data, value);
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