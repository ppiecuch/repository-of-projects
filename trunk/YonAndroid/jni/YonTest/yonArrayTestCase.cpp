#include <cppunit/config/SourcePrefix.h>
#include "yonArrayTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( yonArrayTestCase );

void yonArrayTestCase::setUp()
{
}
void yonArrayTestCase::pushString()
{
	//全局变量没问题
	strArray.push(core::stringc("test"));
	CPPUNIT_ASSERT( strArray.size()==1);

	//局部变量出错
	core::array<core::stringc> temps;
	temps.push(core::stringc("test"));
	CPPUNIT_ASSERT( temps.size()==1);
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

	array<s32> temps;
	temps.push(1);
	CPPUNIT_ASSERT( temps.size()==1);
}

void yonArrayTestCase::insert1()
{
	buffer.push(0);
	buffer.push(1);
	buffer.push(2);
	buffer.push(3);
	buffer.push(4);
	s32 temp[3]={5,6,7};
	buffer.insert(5,temp,3);
	buffer.print();
	CPPUNIT_ASSERT( buffer.size()==8);
}

void yonArrayTestCase::insert2()
{
	buffer.push(0);
	buffer.push(1);
	buffer.push(2);
	buffer.push(3);
	buffer.push(4);
	s32 temp[3]={5,6,7};
	buffer.insert(2,temp,3);
	buffer.print();
	CPPUNIT_ASSERT( buffer.size()==8);
}
void yonArrayTestCase::insert3()
{
	buffer.push(0);
	buffer.push(1);
	buffer.push(2);
	buffer.push(3);
	buffer.push(4);
	s32 temp[3]={5,6,7};
	buffer.insert(1,temp,3);
	buffer.print();
	CPPUNIT_ASSERT( buffer.size()==8);
}
void yonArrayTestCase::insert4()
{
	buffer.push(0);
	buffer.push(1);
	buffer.push(2);
	buffer.push(3);
	buffer.push(4);
	s32 temp[5]={5,6,7,8,9};
	buffer.insert(0,temp,5);
	buffer.print();
	CPPUNIT_ASSERT( buffer.size()==10);
}