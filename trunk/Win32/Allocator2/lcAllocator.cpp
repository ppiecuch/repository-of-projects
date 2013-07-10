#include "lcAllocator.h"
#include <stdlib.h>

namespace lc{

	void* operator new(LC_ALLOC_PARAMS(size_t size)){
	//void* operator new(size_t size, void* ptr, void*){
	//void* operator new(size_t size){
		return malloc(size);
	}

	

	void operator delete(LC_ALLOC_PARAMS(void* ptr)){
		free(ptr);
	}

	/*void* operator new[](size_t size, const c8* file, const c8* func, s32 line){
		return malloc(size);
	}

	void operator delete[](void* ptr, const c8* file, const c8* func, s32 line){
	}*/

}