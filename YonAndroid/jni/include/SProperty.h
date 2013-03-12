#ifndef _YON_GUI_SPROPERTY_H_
#define _YON_GUI_SPROPERTY_H_

#include "variant.h"

namespace yon{
namespace gui{

	struct SProperty{
	public:
		enum ENUM_UNIT
		{
			UNKNOWN = 1 << 0,

			KEYWORD = 1 << 1,			// generic keyword; fetch as < int >

			STRING = 1 << 2,			// generic string; fetch as < String >

			// Absolute values.
			NUMBER = 1 << 3,			// number unsuffixed; fetch as < float >
			PX = 1 << 4,				// number suffixed by 'px'; fetch as < float >
			COLOUR = 1 << 5,			// colour; fetch as < Colourb >
			ABSOLUTE_UNIT = NUMBER | PX | COLOUR,

			//TODO
			// Relative values.
			//EM = 1 << 6,				// number suffixed by 'em'; fetch as < float >
			PERCENT = 1 << 7,			// number suffixed by '%'; fetch as < float >
			//RELATIVE_UNIT = EM | PERCENT,

			//TODO
			// Values based on pixels-per-inch.
			/*INCH = 1 << 8,				// number suffixed by 'in'; fetch as < float >
			CM = 1 << 9,				// number suffixed by 'cm'; fetch as < float >
			MM = 1 << 10,				// number suffixed by 'mm'; fetch as < float >
			PT = 1 << 11,				// number suffixed by 'pt'; fetch as < float >
			PC = 1 << 12,				// number suffixed by 'pc'; fetch as < float >
			PPI_UNIT = INCH | CM | MM | PT | PC*/
		};

		variant Value;
		ENUM_UNIT Unit;
	};
}
}
#endif