#include "objectpoolTestCase.h"
#include <sys/timeb.h>
#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION( objectpoolTestCase );


class CRecyclableObject;
class CObject{
	int a;
	float b;
	//int c[100000];//测试发现随着对象占用的内存空间的扩大，使用new动态生成的方式表现出的效率越差，而对象池则基本保持不变
	friend class CRecyclableObject;
public:
	CObject():a(0),b(0.0f){}
};


class CRecyclableObject:public IRecyclable{
	CObject obj;
public:
	virtual void reset(){obj.a=0;obj.b=0.0f;}
};

void objectpoolTestCase::setUp()
{
}

void objectpoolTestCase::recycle()
{

#define COUNT 100000
	struct _timeb start1;
	_ftime64_s( &start1 ); 
	CObjectPoolNill<CRecyclableObject>* pool1=new CObjectPoolNill<CRecyclableObject>();
	for(int i=0;i<COUNT;++i){
		CRecyclableObject* obj=pool1->get();
		pool1->recycle(obj);
	}
	delete pool1;
	struct _timeb end1;
	_ftime64_s( &end1 );
	printf("\n%d.%d\n",start1.time,start1.millitm);
	printf("%d.%d\n",end1.time,end1.millitm);


	struct _timeb start2;
	_ftime64_s( &start2 ); 

	CObjectPool<CRecyclableObject>* pool2=new CObjectPool<CRecyclableObject>(5);
	for(int i=0;i<COUNT;++i){
		CRecyclableObject* obj=pool2->get();
		pool2->recycle(obj);
	}
	delete pool2;

	struct _timeb end2;
	_ftime64_s( &end2 ); 

	printf("%d.%d\n",start2.time,start2.millitm);
	printf("%d.%d\n",end2.time,end2.millitm);
}