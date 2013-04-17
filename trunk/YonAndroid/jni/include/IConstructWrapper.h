#ifndef _YON_GUI_ICONSTRUCTWRAPPER_H_
#define _YON_GUI_ICONSTRUCTWRAPPER_H_

#include "xmldata.h"

namespace yon{
namespace gui{

	//TODO 如何使用池机制
	//TODO 添加模板参数PARAM

	/**
	* Every concrete Widget class should inherit from this templated class, so they inherit
	* a static method that will create an instance of that class.  This allows us to register this
	* function in a name-function pointer map, for use with serialization.
	*/
	template <class T>
	class IConstructWrapper{
	public:
		static IWidget* createInstance(core::xmldata* data) { return new T(data); }
	};
}
}
#endif