#include "matrixTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( matrixTestCase );

void matrixTestCase::setUp()
{
}

void matrixTestCase::rotateVector(){
	matrix4f m;
	vector3df v;
	m.makeIdentity();
	v.set(1,0,0);
	printf("\n");
	m.rotate(90,0,1,0);
	m.print();
	m.rotateVector(v);
	v.print();
	CPPUNIT_ASSERT( v==core::vector3df(0,0,-1) );
	printf("====================\n");
	m.makeIdentity();
	v.set(0,1,0);
	printf("\n");
	m.rotate(90,1,0,0);
	m.print();
	m.rotateVector(v);
	v.print();
	CPPUNIT_ASSERT( v==core::vector3df(0,0,1) );
	printf("====================\n");
	m.makeIdentity();
	v.set(1,0,0);
	printf("\n");
	m.rotate(90,0,0,1);
	m.print();
	m.rotateVector(v);
	v.print();
	CPPUNIT_ASSERT( v==core::vector3df(0,1,0) );
}