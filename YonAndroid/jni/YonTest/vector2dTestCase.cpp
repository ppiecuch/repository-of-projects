#include "vector2dTestCase.h"
#include <Windows.h>

//CPPUNIT_TEST_SUITE_REGISTRATION( vector2dTestCase );

void vector2dTestCase::setUp()
{
}
void vector2dTestCase::rotateBy(){
	core::vector2df v(1,1);
	v.rotateBy(90);
	v.print();
	CPPUNIT_ASSERT(equals(v.x,-1)&&equals(v.y,1));

	v.set(1,0);
	v.rotateBy(90);
	v.print();
	CPPUNIT_ASSERT(equals(v.x,0)&&equals(v.y,1));

	v.set(1,1);
	v.rotateBy(90,core::vector2df(2,2));
	v.print();
	CPPUNIT_ASSERT(equals(v.x,3)&&equals(v.y,1));
}