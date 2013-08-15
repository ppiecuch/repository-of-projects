#ifndef _MUI_SINGLETON_H_
#define _MUI_SINGLETON_H_

#include "yon.h"

namespace mui{

	template<typename T>
	class Singleton{
	private:
		/** \brief Explicit private copy constructor. This is a forbidden operation.*/
		Singleton(const Singleton<T > &);
		/** \brief Private operator= . This is a forbidden operation. */
		Singleton& operator=(const Singleton<T > &);

	protected:
		static T* s_pSingleton;

		Singleton()
		{
			YON_DEBUG_BREAK_IF( s_pSingleton!=NULL );
//#if defined( LC_CMPL_MSVC ) && LC_CMPL_VERSION < 1200	 
//			s32 offset = (s32)(T*)1 - (s32)(Singleton <T>*)(T*)1;
//			s_pSingleton = (T*)((s32)this + offset);
//#else
			s_pSingleton = static_cast< T* >( this );
//#endif
		}

	public:
		static void create(){
			s_pSingleton = new T;
		}
		virtual void destroy(){
			delete this;
		}
		virtual ~Singleton() {  YON_DEBUG_BREAK_IF( s_pSingleton==NULL );  s_pSingleton = NULL;  }
		

		static T& getInstance() {YON_DEBUG_BREAK_IF( s_pSingleton==NULL );  return ( *s_pSingleton ); }
		static T* getInstancePointer() { return s_pSingleton; }
	};
}
}