#ifndef _LUCID_CORE_SINGLETON_H_
#define _LUCID_CORE_SINGLETON_H_

#include "lcNewAndDelete.h"

namespace lc{
namespace core{

	//使用方法：http://www.kidsang.com/archives/62
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
			LC_DEBUG_BREAK_IF( s_pSingleton!=NULL );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			s32 offset = (s32)(T*)1 - (s32)(Singleton <T>*)(T*)1;
			s_pSingleton = (T*)((s32)this + offset);
#else
			s_pSingleton = static_cast< T* >( this );
#endif
		}

	public:
		//Singleton();
		//virtual ~Singleton();
		static void create(){
			s_pSingleton = LC_NEW T;
		}
		virtual void destroy(){
			delete this;
		}
		virtual ~Singleton() {  LC_DEBUG_BREAK_IF( s_pSingleton==NULL );  s_pSingleton = NULL;  }
		

		static T& getInstance() {LC_DEBUG_BREAK_IF( s_pSingleton==NULL );  return ( *s_pSingleton ); }
		static T* getInstancePointer() { return s_pSingleton; }
	};
}
}
#endif