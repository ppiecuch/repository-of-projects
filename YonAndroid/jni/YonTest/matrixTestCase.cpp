#include "matrixTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( matrixTestCase );

void matrixTestCase::setUp()
{
}
void matrixTestCase::setRotationDegrees(){
	matrix4f m1,m2;
	vector3df v;
	
	m1.makeIdentity();
	m2.makeIdentity();
	m2.rotate(90,1,0,0);
	v.set(90,0,0);
	printf("\n");
	m1.setRotationDegrees(v);
	m1.print();
	printf("\n");
	m2.print();
	CPPUNIT_ASSERT( m1==m2 );

	m1.makeIdentity();
	m2.makeIdentity();
	m2.rotate(90,0,1,0);
	v.set(0,90,0);
	printf("\n");
	m1.setRotationDegrees(v);
	m1.print();
	printf("\n");
	m2.print();
	CPPUNIT_ASSERT( m1==m2 );

	m1.makeIdentity();
	m2.makeIdentity();
	m2.rotate(90,0,0,1);
	v.set(0,0,90);
	printf("\n");
	m1.setRotationDegrees(v);
	m1.print();
	printf("\n");
	m2.print();
	CPPUNIT_ASSERT( m1==m2 );

	m1.makeIdentity();
	m2.makeIdentity();
	m2.rotate(60,0,0,1);
	v.set(0,0,60);
	printf("\n");
	m1.setRotationDegrees(v);
	m1.print();
	printf("\n");
	m2.print();
	CPPUNIT_ASSERT( m1==m2 );
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