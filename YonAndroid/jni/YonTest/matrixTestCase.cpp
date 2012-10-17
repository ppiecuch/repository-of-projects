#include "matrixTestCase.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( matrixTestCase );

void matrixTestCase::setUp()
{
}
void matrixTestCase::transformVect2(){
	matrix4f m1(true);
	m1.setRotationDegrees(core::vector3df(0,90,0));
	core::vector3df v1(1,0,1);
	core::vector3df v2;
	m1.transformVect(v2,v1);
	printf("\n");
	printf("%.2f,%.2f,%.2f\n",v2.x,v2.y,v2.z);
	CPPUNIT_ASSERT(v2==core::vector3df(1,0,-1));

	matrix4f m2(true);
	m2.setRotationDegrees(core::vector3df(0,-90,0));
	core::vector3df v3(0,0,1);
	core::vector3df v4;
	m2.transformVect(v4,v3);
	printf("%.2f,%.2f,%.2f\n",v4.x,v4.y,v4.z);
	CPPUNIT_ASSERT(v4==core::vector3df(-1,0,0));

	matrix4f m3(true);
	m3.setScale(2,2,2);
	core::vector3df v5(1,0,1);
	core::vector3df v6;
	m3.transformVect(v6,v5);
	printf("%.2f,%.2f,%.2f\n",v6.x,v6.y,v6.z);
	CPPUNIT_ASSERT(v6==core::vector3df(2,0,2));
}
void matrixTestCase::getRotationDegrees(){
	matrix4f m1(true);
	core::vector3df r=core::vector3df(30.f,270.f,120.f);
	m1.setRotationDegrees(r);
	m1.translate(11.f,12.f,33.f);
	m1.scale(core::vector3df(2.f,3.f,4.f));
	
	core::vector3df v=m1.getRotationDegrees();
	printf("\n");
	m1.print();
	printf("%.2f,%.2f,%.2f\n",v.x,v.y,v.z);
	

	matrix4f m2(true);
	m2.setRotationDegrees(v);
	m2.translate(11.f,12.f,33.f);
	m2.scale(core::vector3df(2.f,3.f,4.f));
	
	m2.print();

	CPPUNIT_ASSERT(v.equals(r,0.0001f));
}
void matrixTestCase::getScale() {
	matrix4f m1(true);
	core::vector3df v=core::vector3df(1.5f,2.5f,3.5f);
	m1.translate(20,30,40);
	m1.setRotationDegrees(core::vector3df(0,90,0));
	m1.setRotationDegrees(core::vector3df(90,0,0));
	m1.translate(20,30,40);
	m1.setRotationDegrees(core::vector3df(0,0,90));
	m1.scale(v);
	printf("\n");
	core::vector3df s=m1.getScale();
	printf("%.2f,%.2f,%.2f\n",s.x,s.y,s.z);
	CPPUNIT_ASSERT(v==s);

	core::vector3df w=core::vector3df(3.5f,3.5f,3.5f);
	m1.scale(w);
	s=m1.getScale();
	printf("%.2f,%.2f,%.2f\n",s.x,s.y,s.z);
	w*=v;
	CPPUNIT_ASSERT(w==s);
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