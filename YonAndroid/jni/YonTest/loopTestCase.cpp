#include "loopTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( loopTestCase );

void loopTestCase::setUp()
{
}
void loopTestCase::size()
{
	loop<s32> l(10);
	for(s32 i=0;i<10;++i)
	{
		l.supply(i);
		CPPUNIT_ASSERT(l.size()==i&&l.available()==l.capacity()-i-1);
		l.consume(i);
	}
	CPPUNIT_ASSERT(l.size()==0&&l.available()==l.capacity()-1);
	//CPPUNIT_ASSERT(arr[0]==13&&arr[1]==27&&arr[2]==38&&arr[3]==49&&arr[4]==49&&arr[5]==65&&arr[6]==76&&arr[7]==97);
}