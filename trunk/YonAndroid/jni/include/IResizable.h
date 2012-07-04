#ifndef _YON_CORE_IRESIZABLE_H_
#define _YON_CORE_IRESIZABLE_H_

#include "dimension2d.h"

namespace yon{
namespace core{

	class IResizable{
	public:
		virtual void onResize(const core::dimension2du& size) = 0;
	};
}
}
#endif