#include "lcNewAndDelete.h"
#include "lcAllocator.h"
#include "lcLogger.h"

#ifdef LC_SHOW_TRACK
void* operator new(size_t sz){
	LC_WARN("\"new\" operator is deprecated,please use \"LC_NEW\" instead%8s\r\n",LC_SYMBOL_WARN);
	try{
		return malloc(sz);
	}catch(...){
		return NULL;
	}
}
void operator delete(LC_ALLOC_PARAMS(void* ptr)){
	LC_WARN("unexpected delete operator\r\n");
	return free(ptr);
}
#endif
void* operator new(LC_ALLOC_PARAMS(size_t size)){
	//void* operator new(size_t size, void* ptr, void*){
	//void* operator new(size_t size){
	void* p = NULL;
#ifdef LC_SHOW_TRACK
	lc::MemoryTracer::lock();
	//��ʹ��operator new(size)ʱ��Singleton�Ĺ������ô˷�������ʱs_instanceΪNULL��������д��ж�
	if(lc::MemoryTracer::getInstancePointer()&&p)
		lc::MemoryTracer::getInstance().allocate(LC_ALLOC_ARGS_SL(p,size));
	lc::MemoryTracer::unlock();
#endif
	try{
		//throw(std::bad_alloc());
		p = malloc(size);
	}catch(...){}
	return p;
}
void operator delete(void* ptr){
	if(ptr==NULL)
		return; 
#ifdef LC_SHOW_TRACK
	lc::MemoryTracer::lock();
	//Singleton�������н�s_instance��ֵΪNULL��������д��ж�
	if(lc::MemoryTracer::getInstancePointer())
		lc::MemoryTracer::getInstance().deallocate(ptr);
	lc::MemoryTracer::unlock();
#endif
	//��Ϊ��Щָ��Ĺ����Եģ�free���ܱ���
	try{
		free(ptr);
	}catch(...){}
}
