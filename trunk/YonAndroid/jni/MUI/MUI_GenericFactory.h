#ifndef _MUI_GENERICFACTORY_H_
#define _MUI_GENERICFACTORY_H_

#include "MUI_IObject.h"

namespace mui{

	template <typename Type>
	class GenericFactory{
	private:
		static void createFromFactory(IObject*& instance)
		{
			instance = new Type();
		}
	public:
		typedef core::CDelegate1<IObject*&> Delegate;
		static typename Delegate::IDelegate* getFactory()
		{
			return newDelegate(createFromFactory);
		}
	};
}
#endif