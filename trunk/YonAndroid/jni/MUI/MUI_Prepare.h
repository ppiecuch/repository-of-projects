#ifndef _MUI_PREPARE_H_
#define _MUI_PREPARE_H_

#include "MUI_config.h"

#include "yon.h"
using namespace yon;

namespace mui{
	class FactoryManager;
	class IBaseWidget;
	class ICroppedRectangle;
	class ILayer;
	class ILayerNode;
	class IObject;
	class IShapWidget;
	class LayerNode;
	class Widget;
}

#if MUI_COMPILER == MUI_COMPILER_WITH_MSVC
// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable : 4251)
#endif

#endif