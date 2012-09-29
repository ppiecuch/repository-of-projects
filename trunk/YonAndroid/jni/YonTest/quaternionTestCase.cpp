#include "quaternionTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( quaternionTestCase );

void quaternionTestCase::setUp()
{
}
void quaternionTestCase::transformVec()
{
	printf("\n");
	core::vector3df v1(0,0,1);

	core::matrix4f m(true);
	m.rotate(75,1,2,3);
	
	m.transformVect(v1);

	core::quaternion q;
	q.fromAngleAxis(75*DEGTORAD,core::vector3df(1,2,3).normalize());

	core::vector3df v2=q*core::vector3df(0,0,1);

	printf("v1:{%.2f,%.2f,%.2f},v2:{%.2f,%.2f,%.2f}\n",v1.x,v1.y,v1.z,v2.x,v2.y,v2.z);

	CPPUNIT_ASSERT(v1==v2);
}
void quaternionTestCase::slerp()
{
	printf("\n");
	core::vector3df a1(1,0,-1);
	a1.normalize();
	quaternion q1;
	q1.fromAngleAxis(0,a1);

	quaternion q2;
	q2.fromAngleAxis(90*DEGTORAD,a1);

	quaternion q3;
	q3.slerp(q1,q2,0.3333333f);

	f32 angle1;
	core::vector3df a2;
	q3.toAngleAxis(angle1,a2);
	printf("angle1:%.2f,a2:{%.2f,%.2f,%.2f}\n",angle1*RADTODEG,a2.x,a2.y,a2.z);
	CPPUNIT_ASSERT(core::equals(angle1*RADTODEG,30,0.0001f)&&a1.equals(a2,0.0001f));

	quaternion q4;
	q4.slerp(q1,q2,0.6666666f);

	f32 angle2;
	core::vector3df a3;
	q4.toAngleAxis(angle2,a3);
	printf("angle2:%.2f,a3:{%.2f,%.2f,%.2f}\n",angle2*RADTODEG,a3.x,a3.y,a3.z);
	CPPUNIT_ASSERT(core::equals(angle2*RADTODEG,60,0.0001f)&&a1.equals(a3,0.0001f));
}
void quaternionTestCase::Euler()
{
	printf("\n");
	core::vector3df e1(-20*DEGTORAD,80*DEGTORAD,120*DEGTORAD);
	quaternion q1;
	q1.set(e1);

	core::vector3df e2;
	q1.toEuler(e2);
	printf("e1:{%.2f,%.2f,%.2f},e2:{%.2f,%.2f,%.2f}\n",e1.x,e1.y,e1.z,e2.x,e2.y,e2.z);

	CPPUNIT_ASSERT(e1==e2);


	core::vector3df e3(89*DEGTORAD,80*DEGTORAD,120*DEGTORAD);
	quaternion q2;
	q2.set(e3);

	core::vector3df e4;
	q2.toEuler(e4);
	printf("e3:{%.2f,%.2f,%.2f},e4:{%.2f,%.2f,%.2f}\n",e3.x,e3.y,e3.z,e4.x,e4.y,e4.z);

	CPPUNIT_ASSERT(e3==e4);
}
void quaternionTestCase::AngleAxis()
{
	printf("\n");
	quaternion q1;
	core::vector3df a1(-0.68f,0.57f,-0.67f);
	a1.normalize();
	f32 angle1=30;
	q1.fromAngleAxis(angle1*DEGTORAD,a1);

	f32 angle2;
	core::vector3df a2;
	q1.toAngleAxis(angle2,a2);
	printf("angle2:%.2f,a2:{%.2f,%.2f,%.2f}\n",angle2*RADTODEG,a2.x,a2.y,a2.z);
	CPPUNIT_ASSERT(core::equals(angle2*RADTODEG,angle1,0.00001f)&&a1.equals(a2,0.0001f));

	core::matrix4f m1(true);
	m1.setRotation(angle1,a1.x,a1.y,a1.z);
	m1.print();

	core::matrix4f m2=q1.getMatrix();
	m2.print();
	CPPUNIT_ASSERT(m1==m2);
}
void quaternionTestCase::Matrix()
{
	printf("\n");
	matrix4f m1(true);
	core::vector3df r1(120*DEGTORAD,-30*DEGTORAD,90*DEGTORAD);
	m1.setRotationRadians(r1);
	m1.print();

	quaternion q1(r1);
	matrix4f m2=q1.getMatrix();
	m2.print();

	CPPUNIT_ASSERT(m1==m2);

	quaternion q2(m1);
	matrix4f m3=q2.getMatrix();
	m3.print();

	CPPUNIT_ASSERT(m3==m1);
}