#ifndef _YON_CORE_SINGLETON_H_
#define _YON_CORE_SINGLETON_H_

#include "yonTypes.h"

namespace yon{
namespace core{

	template<typename T>
	class singleton{
	private:
		/** \brief Explicit private copy constructor. This is a forbidden operation.*/
		singleton(const singleton<T> &);
		/** \brief Private operator= . This is a forbidden operation. */
		singleton& operator=(const singleton<T> &);

	protected:
		static T* s_pSingleton;

	public:
		singleton()
		{
			YON_DEBUG_BREAK_IF( s_pSingleton!=NULL );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			s32 offset = (s32)(T*)1 - (s32)(singleton <T>*)(T*)1;
			s_pSingleton = (T*)((s32)this + offset);
#else
			s_pSingleton = static_cast< T* >( this );
#endif
		}
		~singleton() {  YON_DEBUG_BREAK_IF( s_pSingleton==NULL );  s_pSingleton = NULL;  }

		static T& getInstance() {YON_DEBUG_BREAK_IF( s_pSingleton==NULL );  return ( *s_pSingleton ); }
		static T* getInstancePointer() { return s_pSingleton; }
	};
}
}
#endif