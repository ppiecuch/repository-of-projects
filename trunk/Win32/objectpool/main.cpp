#include <stdio.h>
#include <list>
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

class CRecyclableObject;
class CObject{
	int a;
	float b;
	friend class CRecyclableObject;
public:
	CObject():a(0),b(0.0f){}
};

class CRecyclableObject:public IRecyclable{
	CObject obj;
public:
	virtual void reset(){obj.a=0;obj.b=0.0f;}
};

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
#else

#define COUNT 1000000
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
	
#endif
	getchar();
	return 0;
}