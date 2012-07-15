#include "matrixTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( matrixTestCase );

void matrixTestCase::setUp()
{
}
void matrixTestCase::transformVect(){
	matrix4f m1(true);
	m1.setRotationDegrees(core::vector3df(0,90,0));
	core::vector3df v1(1,0,1);
	m1.transformVect(v1);
	printf("\n");
	printf("%.2f,%.2f,%.2f\n",v1.x,v1.y,v1.z);
	CPPUNIT_ASSERT(v1==core::vector3df(1,0,-1));

	matrix4f m2(true);
	m2.setRotationDegrees(core::vector3df(0,-90,0));
	core::vector3df v2(0,0,1);
	m2.transformVect(v2);
	printf("%.2f,%.2f,%.2f\n",v2.x,v2.y,v2.z);
	CPPUNIT_ASSERT(v2==core::vector3df(-1,0,0));
}
void matrixTestCase::isIdentity(){
	matrix4f m1;
	CPPUNIT_ASSERT(m1.isIdentity()==false);
	matrix4f m2(true);
	CPPUNIT_ASSERT(m2.isIdentity()==true);
	m1.makeIdentity();
	CPPUNIT_ASSERT(m1.isIdentity()==true);
	m2.makeIdentity();
	CPPUNIT_ASSERT(m2.isIdentity()==true);

	m1.makeTranspose();
	CPPUNIT_ASSERT(m1.isIdentity()==true);
	m2.setTranslation(1,1,1);
	CPPUNIT_ASSERT(m2.isIdentity()==false);

	m1*=m2;
	CPPUNIT_ASSERT(m1.isIdentity()==false);

	m2.setRotation(32,1,0,0);
	CPPUNIT_ASSERT(m2.isIdentity()==false);

	m1.makeIdentity();
	m1*=m2;
	CPPUNIT_ASSERT(m1.isIdentity()==false);

	m2.makeIdentity();
	m2.setScale(2,2,2);
	CPPUNIT_ASSERT(m2.isIdentity()==false);
	m1.makeIdentity();
	m1*=m2;
	CPPUNIT_ASSERT(m1.isIdentity()==false);

	m1.makeIdentity();
	m1.makeInverse();
	CPPUNIT_ASSERT(m1.isIdentity()==true);
	m2.setScale(2,2,2);
	CPPUNIT_ASSERT(m2.isIdentity()==false);

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