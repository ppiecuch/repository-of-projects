#include "lcSingleton.h"

namespace lc{
namespace core{

	//template<typename T>
	//T* Singleton<T>::s_pSingleton=NULL;

	/*template<typename T>
	Singleton<T >::Singleton()
	{
		LC_DEBUG_BREAK_IF( s_pSingleton!=NULL );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		s32 offset = (s32)(T*)1 - (s32)(singleton <T>*)(T*)1;
		s_pSingleton = (T*)((s32)this + offset);
#else
		s_pSingleton = static_cast< T* >( this );
#endif
	}

	template<typename T>
	Singleton<T >::~Singleton() {  LC_DEBUG_BREAK_IF( s_pSingleton==NULL );  s_pSingleton = NULL;  }*/


}
}