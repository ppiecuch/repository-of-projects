#ifndef _LUCID_DEFINE_H_
#define _LUCID_DEFINE_H_

#include "lcConfig.h"

#define LC_VERSION_STRING  \
	LC_TO_STR(LC_VERSION_MAJOR)"." \
	LC_TO_STR(LC_VERSION_MINOR)"." \
	LC_TO_STR(LC_VERSION_PATCH)

#define LC_HOLOS_TEXT \
	"powered by Holos v" LC_VERSION_STRING " (http://lucid.googlecode.com/)"


#define LC_SYMBOL_SUCS		"¡¾¡Ì¡¿"
#define LC_SYMBOL_WARN		"¡¾©u¡¿"
#define LC_SYMBOL_FAIL		"¡¾¨w¡¿"

#ifdef LC_SHOW_OBSOLETE
#define LC_OBSOLETE(text) /*! \deprecated text */ LC_OBSOLETE_START(text)LC_OBSOLETE_END
#else
#define LC_OBSOLETE(text)
#endif


#ifdef LC_SHOW_TRACK
#define LC_ALLOC_PARAMS(...) __VA_ARGS__,const lc::c8* file,const lc::c8* func,lc::s32 line
#define LC_ALLOC_ARGS_MT(...) __VA_ARGS__,LC_FILE,LC_FUNC,LC_LINE
#define LC_ALLOC_ARGS_SL(...) __VA_ARGS__,file,func,line
#define LC_NEW new(LC_FILE,LC_FUNC,LC_LINE)
//delete : cannot delete objects that are not pointers 
//refer to:http://computer-programming-forum.com/81-vc/82637f531ab0897c.htm
//there's no such thing as placement delete
//#define LUCID_DELETE(p) delete(p,LC_FILE,LC_FUNC,LC_LINE)
#else
#define LC_ALLOC_PARAMS(...) __VA_ARGS__
#define LC_ALLOC_ARGS_MT(...) __VA_ARGS__
#define LC_ALLOC_ARGS_SL(...) __VA_ARGS__
#define LC_NEW new
#endif

#define LC_ALLOCATE(allocator,type,sz) allocator.allocate<type>(LC_ALLOC_ARGS_MT(sz))
#define LC_DEALLOCATE(allocator,ptr) allocator.deallocate<void>(LC_ALLOC_ARGS_MT(ptr))



#endif