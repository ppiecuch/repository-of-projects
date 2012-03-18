#include <cppunit/config/SourcePrefix.h>
#include "yonArrayTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( yonArrayTestCase );

void yonArrayTestCase::setUp()
{
}

void yonArrayTestCase::reallocate()
{
	arr.reallocate(36);
	CPPUNIT_ASSERT( arr.size()==0 );
}

void yonArrayTestCase::push()
{
	arr.reallocate(36);
	vector3df* v1=new vector3df(0,1,2);
	vector3df* v2=new vector3df(1,2,3);
	vector3df* v3=new vector3df(2,3,4);
	arr.push(v1);
	arr.push(v2);
	arr.push(v3);
	//delete v;
	CPPUNIT_ASSERT( arr.size()==3&&arr[1]->x==1&&arr[1]->y==2&&arr[1]->z==3 );
}