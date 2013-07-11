#ifndef _LUCID_CORE_SINGLETON_H_
#define _LUCID_CORE_SINGLETON_H_

#include "lcTypes.h"

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
		//MS VC++ 10.0 _MSC_VER = 1600
		//MS VC++ 9.0 _MSC_VER = 1500
		//MS VC++ 8.0 _MSC_VER = 1400
		//MS VC++ 7.1 _MSC_VER = 1310
		//MS VC++ 7.0 _MSC_VER = 1300
		//MS VC++ 6.0 _MSC_VER = 1200
		//MS VC++ 5.0 _MSC_VER = 1100
		//Singleton();
		//virtual ~Singleton();
		static void create(){
			s_pSingleton = new T;
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