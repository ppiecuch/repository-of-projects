#ifndef _YON_YONENGINEPARAMETERS_H_
#define _YON_YONENGINEPARAMETERS_H_

#include "dimension2d.h"

namespace yon{
	struct SYonEngineParameters{

		SYonEngineParameters():
			windowSize(core::dimension2du(800,600))
			{}

		//´°¿Ú³ß´ç
		core::dimension2du windowSize;
	};
}
#endif