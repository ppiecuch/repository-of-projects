#ifndef _YON_GUI_VARIANT_H_
#define _YON_GUI_VARIANT_H_

namespace yon{
namespace gui{

	/**
	Variant is a container that can store a selection of basic types. The variant will store the
	value in the native form corresponding to the version of Set that was called.

	Get is templated to convert from the stored form to the requested form by using a TypeConverter.
	*/
	class variant{
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

	};
}
}
#endif