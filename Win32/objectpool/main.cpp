#include <stdio.h>
#include <list>
#include "objectpool.h"
using namespace std;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <sys/timeb.h>
#include <time.h>

template<class Element,size_t Increment>
class ObjectPool;

class IRecyclable{
public:
	virtual void reset() = 0;
};

class CRecyclable : IRecyclable{
public:
	static unsigned int counter;
	CRecyclable(){++counter;}
	virtual void reset(){}
}; 
unsigned int CRecyclable::counter=0;

template<class Element>
class IObjectPool{
public:
	virtual ~IObjectPool(){}
	virtual unsigned int getSize() const = 0;
	virtual unsigned int getCapacity() const = 0;
	virtual void clear() = 0;
	virtual Element* get() = 0;
	virtual void recycle(Element* ele) = 0;
};

template<class Element>
class CObjectPoolNill : public IObjectPool<Element>{
public:
	virtual unsigned int getSize() const{return 0;}
	virtual unsigned int getCapacity() const{return 0;}
	virtual void clear(){}
	virtual Element* get(){return new Element;}
	virtual void recycle(Element* ele){delete ele;}
};

template<class Element>
class CObjectPool : public IObjectPool<Element>{
protected:
	unsigned int m_uCapacity;
	unsigned int m_uIncrement;
	std::list<Element*> m_pool;

	virtual void extend(unsigned int num){
		for(unsigned int i=0;i<num;++i)
			m_pool.push_back(new Element());
		m_uCapacity+=num;
	}
public:
	CObjectPool(unsigned int increment=5)
		:m_uIncrement(increment),m_uCapacity(0){
			extend(increment);
	}
	virtual ~CObjectPool(){
		clear();
	}
	virtual unsigned int getSize() const {return m_pool.size();}
	virtual unsigned int getCapacity() const {return m_uCapacity;}
	virtual void clear(){
		unsigned int count=getSize();
		std::list<Element*>::iterator it=m_pool.begin();
		while(it!=m_pool.end()){
			delete *it;
			++it;
		}
		m_pool.clear();
		m_uCapacity-=count;
		if(m_uCapacity!=0){
			//Warn
			printf("[WARN]Not all elements recycled\n");
		}
	}
	virtual Element* get(){
		if(getSize()==0)
			extend(m_uIncrement);
		Element* ele=*(m_pool.begin());
		m_pool.pop_front();
		return ele;
	}
	virtual void recycle(Element* ele){
		IRecyclable* r=dynamic_cast<IRecyclable*>(ele);
		if(r){
			r->reset();
			m_pool.push_back(ele);
		}else{
			//Warn
			printf("Recycle no recyclable class,do nothing!\n");
		}
	}
};
template<class Element>
class CObjectPoolSecure : public IObjectPool<Element>{
	//TODO
};
/*
template<class Element,size_t Increment>
class ObjectPool{
protected:
	unsigned int m_uCapacity;
	std::list<Element*> m_pool;

	virtual void extend(unsigned int num){
		for(int i=0;i<num;++i)
			m_pool.push_back(new Element());
		m_uCapacity+=num;
	}
public:
	ObjectPool(unsigned int capacity)
		:m_uCapacity(0){
			extend(capacity);
	}
	virtual ~ObjectPool(){
		clear();
	}
	unsigned int getSize(){return m_pool.size();}
	unsigned int getCapacity(){return m_uCapacity;}
	void clear(){
		unsigned int count=getSize();
		std::list<Element*>::iterator it=m_pool.begin();
		while(it!=m_pool.end()){
			delete *it;
			++it;
		}
		m_pool.clear();
		m_uCapacity-=count;
		if(m_uCapacity!=0){
			//Warn
			printf("[WARN]Not all elements recycled\n");
		}
	}
	Element* get(){
		if(getSize()==0)
			extend(Increment);
		Element* ele=*(m_pool.begin());
		m_pool.pop_front();
		return ele;
	}
	virtual void recycle(Element* ele){
		Element* r=dynamic_cast<Element*>(ele);
		if(r){
			r->reset();
			m_pool.push_back(r);
		}else{
			//Warn
			printf("Recycle no recyclable class,do nothing!\n");
		}
	}
};
*/
class CRecyclableObject;
namespace yon{
	namespace core{
		class CRecyclableObject;
	}
}
class CObject{
	int a;
	float b;
	int c[100000];//测试发现随着对象占用的内存空间的扩大，使用new动态生成的方式表现出的效率越差，而对象池则基本保持不变
	friend class CRecyclableObject;
	friend class yon::core::CRecyclableObject;
public:
	CObject():a(0),b(0.0f){}
};

class CRecyclableObject:public IRecyclable{
	CObject obj;
public:
	virtual void reset(){obj.a=0;obj.b=0.0f;}
};

namespace yon{
	namespace core{

		class CRecyclableObject:public yon::core::IRecyclable{
			CObject obj;
		public:
			virtual void reset(){obj.a=0;obj.b=0.0f;}
		};
	}
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

#if 0
	ObjectPool<CRecyclable,5>* pool=new ObjectPool<CRecyclable,5>(5);
	printf("size:%d,capacity:%d\n",pool->getSize(),pool->getCapacity());

	CRecyclable* i1=pool->get();
	printf("size:%d,capacity:%d,id:%d\n",pool->getSize(),pool->getCapacity(),CRecyclable::counter);

	pool->recycle(i1);
	printf("size:%d,capacity:%d\n",pool->getSize(),pool->getCapacity());

	delete pool;

#elif 0

#define COUNT 100000
	struct _timeb start1;
	_ftime64_s( &start1 ); 
	ObjectPool<CRecyclableObject,5>* pool=new ObjectPool<CRecyclableObject,5>(5);
	for(int i=0;i<COUNT;++i){
		CRecyclableObject* obj=pool->get();
		pool->recycle(obj);
	}
	delete pool;
	struct _timeb end1;
	_ftime64_s( &end1 ); 

	printf("%d.%d\n",start1.time,start1.millitm);
	printf("%d.%d\n",end1.time,end1.millitm);

	struct _timeb start2;
	_ftime64_s( &start2 ); 

	for(int i=0;i<COUNT;++i){
		CObject* obj=new CObject();
		delete obj;
	}

	struct _timeb end2;
	_ftime64_s( &end2 ); 

	printf("%d.%d\n",start2.time,start2.millitm);
	printf("%d.%d\n",end2.time,end2.millitm);
#else

#define COUNT 100000
	struct _timeb start;
	struct _timeb end;
	long long s;
	int ms;

	//12.***ms
	_ftime64_s( &start ); 
	CObjectPoolNill<CRecyclableObject>* pool1=new CObjectPoolNill<CRecyclableObject>();
	for(int i=0;i<COUNT;++i){
		CRecyclableObject* obj=pool1->get();
		pool1->recycle(obj);
	}
	delete pool1;
	_ftime64_s( &end );

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%d\n",ms);


	//0.672ms
	_ftime64_s( &start ); 
	CObjectPool<CRecyclableObject>* pool2=new CObjectPool<CRecyclableObject>(5);
	for(int i=0;i<COUNT;++i){
		CRecyclableObject* obj=pool2->get();
		pool2->recycle(obj);
	}
	delete pool2;
	_ftime64_s( &end ); 

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%d\n",ms);

	//0.172ms
	_ftime64_s( &start ); 
	yon::core::CObjectPool<yon::core::CRecyclableObject>* pool3=new yon::core::CObjectPool<yon::core::CRecyclableObject>(5);
	for(int i=0;i<COUNT;++i){
		yon::core::CRecyclableObject* obj=pool3->get();
		pool3->recycle(obj);
	}
	delete pool3;
	_ftime64_s( &end ); 

	s=end.time-start.time;
	ms=end.millitm-start.millitm;
	if(ms<0){
		ms+=1000;
		--s;
	}
	printf("%d.",s);
	printf("%d\n",ms);
#endif
	system("pause");
	return 0;
}